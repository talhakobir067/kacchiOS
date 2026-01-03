#include "process.h"
#include "memory.h"

static pcb_t proc_table[MAX_PROC];
static int curr_pid = -1;

void process_init() {
    for (int i = 0; i < MAX_PROC; i++)
        proc_table[i].state = TERMINATED;
}

int process_create(void (*entry)()) {
    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state == TERMINATED) {
            proc_table[i].pid = i;
            proc_table[i].state = READY;
            proc_table[i].entry = entry;
            proc_table[i].priority = 1;
            proc_table[i].age = 0;
            proc_table[i].msg = 0;

            uint32_t* stack = (uint32_t*)kmalloc(STACK_SIZE);
            proc_table[i].sp = stack + STACK_SIZE / 4 - 1;

            return i;
        }
    }
    return -1;
}

void process_terminate(int pid) {
    proc_table[pid].state = TERMINATED;
    kfree(proc_table[pid].sp);
}

pcb_t* process_get(int pid) {
    return &proc_table[pid];
}

pcb_t* current_process() {
    if (curr_pid < 0) return 0;
    return &proc_table[curr_pid];
}
