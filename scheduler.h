#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_

#include "process.h"

#define FIFO	1
#define RR	2
#define SJF	3
#define PSJF	4


/** 
 * Schedule all the process
 * Check if process ready and execute,then chooese the next process,
 * run an unit of time and reapeat untill all the process are scheduled
 * @param proc is the set of process that are prepared to be scheduled
 * @param nporc is number of process
 * @param policy is the kind of scheduling algorithm, such as RR,SJF...
 * @return 0 if successful
*/
int scheduling(struct process *proc, int nproc, int policy);

#endif
