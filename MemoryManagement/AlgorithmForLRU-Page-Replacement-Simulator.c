#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAM_SIZE 4       // Number of physical frames in RAM
#define PAGE_SIZE 1024   // Bytes per page (simulation only)
#define NUM_PAGES 8      // Virtual pages per process
#define NUM_PROCESSES 2  // Number of processes
#define NUM_ACCESSES 20  // Number of memory accesses to simulate

typedef struct {
    int frame_number;  // physical frame in RAM
    int present;       // 1 if in RAM, 0 if swapped out
    int modified;      // Dirty bit
    int last_access;   // Last access time (for LRU)
} PageTableEntry;

typedef struct {
    PageTableEntry *page_table;
} Process;

Process processes[NUM_PROCESSES];
int ram_owner_proc[RAM_SIZE];   // Which process owns the frame
int ram_owner_page[RAM_SIZE];   // Which virtual page is stored
int last_access_time[RAM_SIZE]; // Time when the frame was last accessed
int free_frame_list[RAM_SIZE];  // Free frame list (1 = free, 0 = used)

int current_time = 0;           // Simulated time counter
int page_fault_count = 0;
int page_hit_count = 0;

// -------------------- Step 1: Initialization --------------------
void initialize_system() {
    srand(time(NULL));
    for (int p = 0; p < NUM_PROCESSES; p++) {
        processes[p].page_table = malloc(sizeof(PageTableEntry) * NUM_PAGES);
        for (int pg = 0; pg < NUM_PAGES; pg++) {
            processes[p].page_table[pg].frame_number = -1;
            processes[p].page_table[pg].present = 0;
            processes[p].page_table[pg].modified = 0;
            processes[p].page_table[pg].last_access = -1;
        }
    }
    for (int f = 0; f < RAM_SIZE; f++) {
        ram_owner_proc[f] = -1;
        ram_owner_page[f] = -1;
        last_access_time[f] = -1;
        free_frame_list[f] = 1; // 1 = free
    }
    printf("System initialized: %d processes, %d pages each, %d RAM frames.\n",
           NUM_PROCESSES, NUM_PAGES, RAM_SIZE);
}

// -------------------- Step 3: Page Fault Handling (LRU) --------------------
void handle_page_fault(int proc_id, int page_id) {
    page_fault_count++;
    printf("PAGE FAULT: Process %d needs page %d\n", proc_id, page_id);

    // Check for a free frame
    int free_frame = -1;
    for (int f = 0; f < RAM_SIZE; f++) {
        if (free_frame_list[f] == 1) {
            free_frame = f;
            break;
        }
    }

    if (free_frame != -1) {
        printf("Loading into free frame %d\n", free_frame);
    } else {
        // LRU replacement
        int lru_frame = 0;
        for (int f = 1; f < RAM_SIZE; f++) {
            if (last_access_time[f] < last_access_time[lru_frame]) {
                lru_frame = f;
            }
        }
        free_frame = lru_frame;
        int victim_proc = ram_owner_proc[free_frame];
        int victim_page = ram_owner_page[free_frame];
        printf("Replacing frame %d (Proc %d, Page %d)\n",
               free_frame, victim_proc, victim_page);

        // Write back if modified
        if (processes[victim_proc].page_table[victim_page].modified) {
            printf("Writing dirty page back to disk...\n");
        }

        // Remove victim page from RAM
        processes[victim_proc].page_table[victim_page].present = 0;
        processes[victim_proc].page_table[victim_page].frame_number = -1;
    }

    // Load new page into frame
    processes[proc_id].page_table[page_id].present = 1;
    processes[proc_id].page_table[page_id].frame_number = free_frame;
    processes[proc_id].page_table[page_id].last_access = current_time;
    processes[proc_id].page_table[page_id].modified = rand() % 2;

    // Update RAM metadata
    ram_owner_proc[free_frame] = proc_id;
    ram_owner_page[free_frame] = page_id;
    last_access_time[free_frame] = current_time;
    free_frame_list[free_frame] = 0;
}

// -------------------- Step 2: Simulate Memory Access --------------------
void access_page(int proc_id, int page_id) {
    PageTableEntry *pte = &processes[proc_id].page_table[page_id];
    if (pte->present) {
        page_hit_count++;
        printf("Process %d accessed page %d in frame %d (HIT)\n",
               proc_id, page_id, pte->frame_number);
        pte->last_access = current_time;
        last_access_time[pte->frame_number] = current_time;
    } else {
        handle_page_fault(proc_id, page_id);
    }
}

void simulate_accesses() {
    for (int i = 0; i < NUM_ACCESSES; i++) {
        int proc_id = rand() % NUM_PROCESSES;
        int page_id = rand() % NUM_PAGES;
        current_time++;
        access_page(proc_id, page_id);
    }
}

// -------------------- Step 4: Final State Dump --------------------
void dump_state() {
    printf("\n--- Final RAM Frames State ---\n");
    for (int f = 0; f < RAM_SIZE; f++) {
        if (ram_owner_proc[f] != -1) {
            printf("Frame %d: Process %d, Page %d, Last Access %d\n",
                   f, ram_owner_proc[f], ram_owner_page[f], last_access_time[f]);
        } else {
            printf("Frame %d: Free\n", f);
        }
    }

    printf("\n--- Final Page Tables ---\n");
    for (int p = 0; p < NUM_PROCESSES; p++) {
        printf("Process %d:\n", p);
        for (int pg = 0; pg < NUM_PAGES; pg++) {
            PageTableEntry *pte = &processes[p].page_table[pg];
            printf("  Page %d: Present=%d, Frame=%d, Modified=%d, LastAccess=%d\n",
                   pg, pte->present, pte->frame_number, pte->modified, pte->last_access);
        }
    }

    printf("\n--- Simulation Statistics ---\n");
    printf("Total Memory Accesses: %d\n", NUM_ACCESSES);
    printf("Page Hits: %d\n", page_hit_count);
    printf("Page Faults: %d\n", page_fault_count);
    printf("Hit Ratio: %.2f%%\n", (100.0 * page_hit_count) / NUM_ACCESSES);
}

// -------------------- Cleanup --------------------
void cleanup_system() {
    for (int p = 0; p < NUM_PROCESSES; p++) {
        free(processes[p].page_table);
    }
}

// -------------------- Main --------------------
int main(void) {
    initialize_system();
    simulate_accesses();
    dump_state();
    cleanup_system();
    return 0;
}
