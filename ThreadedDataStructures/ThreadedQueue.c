// fg_queue_test.c
// Fine-Grained Locking MPMC queue (per-node lock + tail lock + head lock)
// Test harness included.
//
// Compile:
//   gcc -std=c11 fg_queue_test.c -pthread -o fg_queue_test
//
// Run:
//   ./fg_queue_test

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <assert.h>
#include <errno.h>

// -------------------- Queue implementation --------------------

typedef struct fg_node {
    void *value;
    struct fg_node *next;
    mtx_t lock;        // per-node lock
} fg_node_t;

typedef struct {
    fg_node_t *head;   // points to dummy node
    fg_node_t *tail;
    mtx_t tail_lock;   // protects tail pointer and tail->next insertion
    mtx_t head_lock;   // serialize dequeuers for safety
    atomic_size_t size; // approximate size (updated with atomic ops)
} fg_queue_t;

// Initialize queue. Returns 0 on success.
int fgq_init(fg_queue_t *q) {
    if (!q) return -1;
    fg_node_t *dummy = malloc(sizeof(fg_node_t));
    if (!dummy) return -1;
    dummy->value = NULL;
    dummy->next = NULL;
    if (mtx_init(&dummy->lock, mtx_plain) != thrd_success) {
        free(dummy);
        return -1;
    }
    q->head = q->tail = dummy;
    if (mtx_init(&q->tail_lock, mtx_plain) != thrd_success) {
        mtx_destroy(&dummy->lock);
        free(dummy);
        return -1;
    }
    if (mtx_init(&q->head_lock, mtx_plain) != thrd_success) {
        mtx_destroy(&q->tail_lock);
        mtx_destroy(&dummy->lock);
        free(dummy);
        return -1;
    }
    atomic_store(&q->size, 0);
    return 0;
}

// Destroy queue. Queue must be empty (no other threads).
void fgq_destroy(fg_queue_t *q) {
    if (!q) return;
    // Free remaining nodes (should only be dummy in a correct test)
    fg_node_t *n = q->head;
    while (n) {
        fg_node_t *next = n->next;
        mtx_destroy(&n->lock);
        free(n);
        n = next;
    }
    mtx_destroy(&q->tail_lock);
    mtx_destroy(&q->head_lock);
}

// Enqueue a value (pointer). Caller transfers ownership of value to queue.
void fgq_enqueue(fg_queue_t *q, void *value) {
    fg_node_t *node = malloc(sizeof(fg_node_t));
    if (!node) {
        perror("malloc");
        abort();
    }
    node->value = value;
    node->next = NULL;
    if (mtx_init(&node->lock, mtx_plain) != thrd_success) {
        perror("mtx_init");
        free(node);
        abort();
    }

    // Link the new node at the tail under tail_lock
    mtx_lock(&q->tail_lock);
    q->tail->next = node;
    q->tail = node;
    mtx_unlock(&q->tail_lock);

    atomic_fetch_add(&q->size, 1);
}

// Dequeue a value. Returns value pointer or NULL if queue empty.
// Caller owns returned pointer (must free if heap allocated).
void *fgq_dequeue(fg_queue_t *q) {
    fg_node_t *old_head;
    void *ret = NULL;

    // Ensure only one dequeuer at a time moves the head pointer.
    mtx_lock(&q->head_lock);

    old_head = q->head;
    // We lock the old_head node to inspect its next safely.
    mtx_lock(&old_head->lock);

    fg_node_t *next = old_head->next;
    if (next == NULL) {
        // empty
        mtx_unlock(&old_head->lock);
        mtx_unlock(&q->head_lock);
        return NULL;
    }

    // lock next so we can safely read its value and move head
    mtx_lock(&next->lock);

    // Move head forward
    q->head = next;

    // Extract value
    ret = next->value;
    next->value = NULL; // clear

    // unlock next (it is the new dummy head; keep its lock valid)
    mtx_unlock(&next->lock);

    // unlock and destroy old head's lock then free it
    mtx_unlock(&old_head->lock);
    mtx_destroy(&old_head->lock);
    free(old_head);

    atomic_fetch_sub(&q->size, 1);

    // done moving head; allow other dequeuers
    mtx_unlock(&q->head_lock);

    return ret;
}

// Approximate size (may be slightly off)
size_t fgq_size(fg_queue_t *q) {
    return (size_t)atomic_load(&q->size);
}

// -------------------- Test harness --------------------

typedef struct {
    fg_queue_t *queue;
    int id;
    int items;
    atomic_int *produced_count;
} producer_arg_t;

typedef struct {
    fg_queue_t *queue;
    int id;
    int total_to_consume;
    atomic_int *consumed_count;
} consumer_arg_t;

#define PRODUCERS 4
#define CONSUMERS 4
#define ITEMS_PER_PRODUCER 100000  // adjust for heavier test

atomic_int produced_total;
atomic_int consumed_total;

int producer_thread(void *arg) {
    producer_arg_t *pa = (producer_arg_t*)arg;
    for (int i = 0; i < pa->items; ++i) {
        // produce an integer stored on heap
        int *p = malloc(sizeof(int));
        if (!p) abort();
        // value encodes producer id and index
        *p = (pa->id << 24) ^ i;
        fgq_enqueue(pa->queue, p);
        atomic_fetch_add(pa->produced_count, 1);
    }
    return 0;
}

int consumer_thread(void *arg) {
    consumer_arg_t *ca = (consumer_arg_t*)arg;
    while (1) {
        void *v = fgq_dequeue(ca->queue);
        if (v) {
            // process (we just free and count)
            free(v);
            int cons = atomic_fetch_add(ca->consumed_count, 1) + 1;
            if (cons >= ca->total_to_consume) break;
        } else {
            // queue empty. Check if production finished
            int prod = atomic_load(&produced_total);
            int cons = atomic_load(&consumed_total);
            if (prod >= ca->total_to_consume && cons >= ca->total_to_consume) {
                break;
            }
            // small sleep to avoid busy spin
            struct timespec ts = {0, 1000}; // 1 µs
            thrd_sleep(&ts, NULL);
        }
    }
    return 0;
}

int main(void) {
    printf("Fine-Grained Locking Queue test\n");
    fg_queue_t q;
    if (fgq_init(&q) != 0) {
        fprintf(stderr, "Failed to init queue\n");
        return 1;
    }

    atomic_init(&produced_total, 0);
    atomic_init(&consumed_total, 0);

    int total_items = PRODUCERS * ITEMS_PER_PRODUCER;
    printf("Producers: %d, Consumers: %d, Items total: %d\n",
           PRODUCERS, CONSUMERS, total_items);

    // Create producer threads
    thrd_t producers[PRODUCERS];
    producer_arg_t pargs[PRODUCERS];
    for (int i = 0; i < PRODUCERS; ++i) {
        pargs[i].queue = &q;
        pargs[i].id = i;
        pargs[i].items = ITEMS_PER_PRODUCER;
        pargs[i].produced_count = &produced_total;
        if (thrd_create(&producers[i], producer_thread, &pargs[i]) != thrd_success) {
            fprintf(stderr, "Failed to create producer %d\n", i);
            return 1;
        }
    }

    // Create consumer threads
    thrd_t consumers[CONSUMERS];
    consumer_arg_t cargs[CONSUMERS];
    for (int i = 0; i < CONSUMERS; ++i) {
        cargs[i].queue = &q;
        cargs[i].id = i;
        cargs[i].total_to_consume = total_items;
        cargs[i].consumed_count = &consumed_total;
        if (thrd_create(&consumers[i], consumer_thread, &cargs[i]) != thrd_success) {
            fprintf(stderr, "Failed to create consumer %d\n", i);
            return 1;
        }
    }

    // Wait for producers
    for (int i = 0; i < PRODUCERS; ++i) {
        thrd_join(producers[i], NULL);
    }
    printf("All producers finished. Produced = %d\n", atomic_load(&produced_total));

    // Wait for consumers
    for (int i = 0; i < CONSUMERS; ++i) {
        thrd_join(consumers[i], NULL);
    }
    printf("All consumers finished. Consumed = %d\n", atomic_load(&consumed_total));

    // Final size should be 0
    printf("Final queue size (approx): %zu\n", fgq_size(&q));
    assert(fgq_size(&q) == 0);

    fgq_destroy(&q);
    printf("Test completed OK\n");
    return 0;
}
