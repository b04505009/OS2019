#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>

#define CHILD_CPU 1
#define PARENT_CPU 0
#define LOOPS_PER_UNIT 1000000UL

/* Running one unit time */
#define UNIT_T()				\
{						\
		volatile unsigned long i;		\
		for (i = 0; i < LOOPS_PER_UNIT; i++);	\
}						\
/**
 * construct a process with given name, ready time, execution time,  
 * and the id of the process
*/
struct process {
		char name[32];  //ex: P0,P1,...
		int t_ready;	//ready time
		int t_exec;		//execition time(running time)
		pid_t pid;		//process' ID
};

/**
 * Assign process to specific core 
 * @param pid is the id of a process
 * @param core is the core you want to use
 * @return 0 if the process correctly assigned
*/
int proc_assign_cpu(int pid, int core);

/**
 * Execute the process, using fork() to generate child process.
 * Display the process' name, starting time ,and end time
 * @param proc is the process to be executed
 * @retrun the execution process' id
*/
int proc_exec(struct process proc);

/**
 * Set very low priority to process
 * using function sched_setscheduler() 
 * @param pid is the id of the porecee you want to block
 * @return 0 if the operation is well done
*/
int proc_block(int pid);

/** 
 * Set high priority to proces
 * using function sched_setscheduler()
 * @param pid is the id of the porecee you want to wake
 * @return 0 if the operation is well done
 */
int proc_wakeup(int pid);

#endif
