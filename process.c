#define _GNU_SOURCE
#include <sched.h>  // for sched_setaffinity, sched_setscheduler, sched_param
#include <stdio.h>  // for perror
#include <unistd.h> // for sysconf(_SC_NPROCESSORS_CONF)
#include <stdlib.h> // for exit(1)
#include <sys/syscall.h>    // syscall
#include <time.h>
#include <sys/time.h>
#include "process.h"

#include <linux/kernel.h>

#define KERN_INFO "<6>"
#define PRINTK 315

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
    if (result < 0){
        perror("cound not get thread affinity, continuing...\n");
        exit(1);
    }
    else {
        //long nprocessor = sysconf(_SC_NPROCESSORS_CONF);    // number of cores are online
        //printf("sched_getaffinity = ");
        //for (long i = 0; i < nprocessor; i++) {
        //    printf("%d ", CPU_ISSET(i, &mask)); // Check if core i has been add to the cpuset
        //}
        //printf("\n");
        return result;
    }
}



int proc_exec(struct process proc){
    struct timespec start_time, end_time;
    int pid = fork();
    if (pid < 0){
        perror("create thread fail.");
        return -1;
    } 
    else if (pid == 0){
        clock_gettime(CLOCK_REALTIME, &start_time); // get cpu time when process start
        char to_dmesg[200];
        for (int i = 0; i < proc.t_exec; i++) {
			UNIT_T();
#ifdef DEBUG
			if (i % 100 == 0)
				fprintf(stderr, "%s: %d/%d\n", proc.name, i, proc.t_exec);
#endif
		}
        clock_gettime(CLOCK_REALTIME, &end_time); // get cpu time when process end
        sprintf(to_dmesg, "echo \"[project1] %d %lu.%09lu %lu.%09lu\n\" > /dev/kmsg", getpid(), start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);
        //sprintf(to_dmesg, "[project1] %d %lu.%09lu %lu.%09lu\n", getpid(), start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);
        system(to_dmesg); // print string to_dmesg dmesg
        //printk(KERN_INFO "[project1] %d %lu.%09lu %lu.%09lu\n", getpid(), start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);
        exit(0);
    }
    proc_assign_cpu(pid, CHILD_CPU);
    return pid;
}

// Block: set very low priority to pid
int proc_block(int pid){
    struct sched_param param;
    param.__sched_priority = 0; // param->sched_priority must be 0 for SCHED_OTHER, SCHED_BATCH, SCHED_IDLE.
    int result = sched_setscheduler(pid, SCHED_IDLE, &param);   // sets scheduling policy and parameters for pid
    if (result < 0){
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
    if (result < 0){
        perror("sched_setscheduler wakeup(time-sharing RR) fail.");
        return -1;
    }
    else{
        return 0;
    }
}


