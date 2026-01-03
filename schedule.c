#include "scheduler.h"
#include "process.h"

static int time_quantum;
static int ticks;

void scheduler_init(int quantum) {
    time_quantum = quantum;
    ticks = 0;
    process_init();
}

void scheduler_tick() {
    ticks++;

    // Aging
    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state == READY)
            proc_table[i].age++;
    }

    if (ticks < time_quantum) return;
    ticks = 0;

    int next = -1;
    int max_age = -1;

    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state == READY &&
            proc_table[i].age > max_age) {
            max_age = proc_table[i].age;
            next = i;
        }
    }

    if (next < 0) return;

    if (curr_pid >= 0 &&
        proc_table[curr_pid].state == RUNNING)
        proc_table[curr_pid].state = READY;

    curr_pid = next;
    proc_table[next].state = RUNNING;
    proc_table[next].age = 0;

    proc_table[next].entry();
}
