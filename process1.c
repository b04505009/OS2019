#define _GNU_SOURCE
#include <sched.h>  // for sched_setaffinity, sched_setscheduler, sched_param
#include <stdio.h>  // for perror
#include <unistd.h> // for sysconf(_SC_NPROCESSORS_CONF)
#include <stdlib.h> // for exit(1)
#include <sys/syscall.h>    // syscall
#include "process.h"


// Set process afinity to cpu
int proc_assign_cpu(int pid, int core){
    if (core > (int)sysconf(_SC_NPROCESSORS_CONF)){
        perror("number of core out of scale");
        exit(1);
    }
    cpu_set_t  mask;    // CPU set
    CPU_ZERO(&mask);    // initialize, remove all CPUs from cpuset
    CPU_SET(core, &mask);   // add CPU core to the cpuset
    int result = sched_setaffinity(pid, sizeof(mask), &mask);  // 0 for successful, -1 for error 
    if (result == -1){
        perror("cound not get thread affinity, continuing...\n");
        exit(1);
    }
    else {
        long nprocessor = sysconf(_SC_NPROCESSORS_CONF);    // number of cores are online
        printf("sched_getaffinity = ");
        for (long i = 0; i < nprocessor; i++) {
            printf("%d ", CPU_ISSET(i, &mask)); // Check if core i has been add to the cpuset
        }
        printf("\n");
        return result;
    }
}


int proc_exec(struct process proc){
    int pid = fork();
    if (pid < 0){
        perror("create thread fail.");
        return -1;
    }
    else if (pid == 0){


    }
}

// Block: set very low priority to pid
int proc_block(int pid){
    struct sched_param param;
    param.__sched_priority = 0; // param->sched_priority must be 0 for SCHED_OTHER, SCHED_BATCH, SCHED_IDLE.
    int result = sched_setscheduler(pid, SCHED_IDLE, &param);   // sets scheduling policy and parameters for pid
    if (result == -1){
        perror("sched_setscheduler idle fail.");
        return -1;
    }
    else{
        return 0;
    }
}

// Wakeup: set priority to maximum
int proc_wakeup(int pid){
    struct sched_param param;
    param.__sched_priority = 0;
    int result = sched_setscheduler(pid, SCHED_OTHER, &param);
    if (result == -1){
        perror("sched_setscheduler wakeup(time-sharing RR) fail.");
        return -1;
    }
    else{
        return 0;
    }
}


