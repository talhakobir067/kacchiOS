#include "scheduler.h"
#include "process.h"

static int timeSlice;
static int tickCounter;

void scheduler_init(int quantum) {
    timeSlice = quantum;
    tickCounter = 0;
    process_init();
}

void scheduler_tick() {
    tickCounter++;

    if (tickCounter < timeSlice)
        return;

    tickCounter = 0;

    int selectedProcess = -1;

    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state == READY) {
            selectedProcess = i;
            break;
        }
    }

    if (selectedProcess < 0)
        return;

    if (curr_pid >= 0 &&
        proc_table[curr_pid].state == RUNNING) {

        proc_table[curr_pid].state = READY;
    }

    curr_pid = selectedProcess;
    proc_table[selectedProcess].state = RUNNING;

    proc_table[selectedProcess].entry();
}
