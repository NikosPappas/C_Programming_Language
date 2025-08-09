#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BLOCK_SIZE 64 // simulated payload size

// Represents a block in memory. The heap_index is critical for efficient updates.
typedef struct {
    int id;
    uint64_t last_access;
    char data[BLOCK_SIZE];
    int heap_index; // position in the heap
} MemoryBlock;

// A single struct to hold the entire state of the simulation.
// This avoids passing multiple disconnected pointers around.
typedef struct {
    MemoryBlock *ram;       // The underlying contiguous memory blocks
    MemoryBlock **heap;     // The min-heap (array of pointers into ram)
    int heap_size;          // Current number of items in the heap
    int capacity;           // The total number of blocks (capacity of ram/heap)
    uint64_t current_time;
} SimulationState;

// A function pointer type that defines how the next block to access is chosen.
typedef int (*AccessStrategy)(const SimulationState *state);

// --- Forward Declarations ---
void swap_heap_nodes(MemoryBlock **heap, int i, int j);
void heapify_up(MemoryBlock **heap, int index);
void heapify_down(MemoryBlock **heap, int index, int heap_size);

// --- Initialization and Cleanup ---

// Initializes the entire simulation state. This is the "impure" part of the
// code where all memory allocation and initial state setup occurs.
SimulationState init_simulation(int num_blocks) {
    SimulationState state;
    state.capacity = num_blocks;
    state.heap_size = num_blocks;
    state.current_time = 0;

    // Allocate the contiguous RAM block
    state.ram = malloc(sizeof(MemoryBlock) * num_blocks);
    if (!state.ram) {
        fprintf(stderr, "Error: could not allocate RAM.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate the heap structure (pointers to RAM)
    state.heap = malloc(sizeof(MemoryBlock*) * num_blocks);
    if (!state.heap) {
        free(state.ram);
        fprintf(stderr, "Error: could not allocate heap structure.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize each memory block and set up the heap pointers
    for (int i = 0; i < num_blocks; i++) {
        uint64_t init_time = rand() % 1000;
        state.ram[i] = (MemoryBlock){.id = i, .last_access = init_time, .heap_index = i};
        state.heap[i] = &state.ram[i];
    }

    // Build the initial min-heap from the randomized access times
    for (int i = state.capacity / 2 - 1; i >= 0; i--) {
        heapify_down(state.heap, i, state.heap_size);
    }
    
    return state;
}

// Frees all resources associated with the simulation state.
void cleanup_simulation(SimulationState *state) {
    free(state->heap);
    free(state->ram);
    state->heap = NULL;
    state->ram = NULL;
}

// --- Core Heap Logic (largely unchanged, but used by state-aware functions) ---

void swap_heap_nodes(MemoryBlock **heap, int i, int j) {
    MemoryBlock *tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
    heap[i]->heap_index = i;
    heap[j]->heap_index = j;
}

void heapify_up(MemoryBlock **heap, int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && heap[index]->last_access < heap[parent]->last_access) {
        swap_heap_nodes(heap, index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void heapify_down(MemoryBlock **heap, int index, int heap_size) {
    while (1) {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int smallest = index;

        if (left < heap_size && heap[left]->last_access < heap[smallest]->last_access) {
            smallest = left;
        }
        if (right < heap_size && heap[right]->last_access < heap[smallest]->last_access) {
            smallest = right;
        }
        if (smallest != index) {
            swap_heap_nodes(heap, index, smallest);
            index = smallest;
        } else {
            break;
        }
    }
}

// --- State Transition Functions ---

// Updates a block's access time and restores the heap property.
// This function now operates on the state's heap.
void update_block_access(SimulationState *state, MemoryBlock *block, uint64_t new_time) {
    uint64_t old_time = block->last_access;
    block->last_access = new_time;

    if (new_time > old_time) {
        heapify_down(state->heap, block->heap_index, state->heap_size);
    } else {
        heapify_up(state->heap, block->heap_index);
    }
}

// Gets the current Least Recently Used block without removing it.
// This is a "pure" function as it only reads from the state.
const MemoryBlock* peek_lru(const SimulationState *state) {
    if (state->heap_size <= 0) return NULL;
    return state->heap[0]; // The root of the min-heap is the LRU
}

// A strategy for choosing which block to "access" next.
// This one just picks a random block.
int random_access_strategy(const SimulationState *state) {
    if (state->heap_size == 0) return -1;
    return rand() % state->heap_size;
}

// This function represents a single, self-contained step of the simulation.
// It takes the current state and a strategy, and transitions the state forward.
void simulation_step(SimulationState *state, AccessStrategy find_next_block) {
    state->current_time++;
    
    int heap_idx_to_access = find_next_block(state);
    if (heap_idx_to_access == -1) return; // Nothing to do

    MemoryBlock *block_to_update = state->heap[heap_idx_to_access];
    update_block_access(state, block_to_update, state->current_time);
}

// --- Main Program ---

int main() {
    srand(time(NULL));

    int num_blocks = 1000;
    int simulation_ticks = 10000;

    // 1. Initialize the entire system state
    SimulationState sim_state = init_simulation(num_blocks);

    printf("Starting simulation with %d blocks for %d ticks.\n\n", num_blocks, simulation_ticks);

    // 2. The main loop is now much cleaner. It passes the state
    //    to a function that calculates the next state.
    for (int tick = 0; tick < simulation_ticks; tick++) {
        // The core logic of a single step
        simulation_step(&sim_state, random_access_strategy);

        // Peeking at the result is a separate, read-only action
        const MemoryBlock* lru_block = peek_lru(&sim_state);
        
        // Side-effects (like printing) are kept in the main loop
        printf("Time %llu | LRU block ID: %d (last_access = %llu)\n",
               sim_state.current_time, lru_block->id, lru_block->last_access);
    }

    // 3. Clean up all resources associated with the state
    cleanup_simulation(&sim_state);

    return 0;
}
