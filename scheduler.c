#define _GNU_SOURCE
#include "process.h"
#include "scheduler.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sched.h>
#define TIME_QUANTUM 500

/* Last context switch time for RR scheduling */
static int t_last;

/* Current unit time */
static int ntime;

/* Index of running process. -1 if no process running */
static int running;

/* Number of finish Process */
static int finish_cnt;

/** 
 * Sort processes by ready time 
 * sort ready time in increasing order
*/
int cmp_proc_fcfs(const void *a, const void *b)
{
		if (((struct process *)a)->t_ready != ((struct process *)b)->t_ready)
				return ((struct process *)a)->t_ready - ((struct process *)b)->t_ready;
		else
				return strcmp(((struct process *)a)->name, ((struct process *)b)->name);
}
/**
 * Sort processes by remain time
 * sort ready time in increasing order
*/
int cmp_proc_sjf(const void *a, const void *b)
{
	
		if (((struct process *)a)->t_exec != ((struct process *)b)->t_exec)
				return ((struct process *)a)->t_exec - ((struct process *)b)->t_exec;
		else
				return strcmp(((struct process *)a)->name, ((struct process *)b)->name);
}

/** 
 * Distinguish which the next process to be called by 
 * the different scheduling algorithms 
 * @param proc is the set of process that are prepared to be scheduled
 * @param nporc is number of process
 * @param policy is the kind of scheduling algorithm, such as RR,SJF...
 * @return index of next process
*/
int next_process(struct process *proc, int nproc, int policy)
{
		/* Non-preemptive */
		if (running != -1 && (policy == SJF || policy == FIFO))
				return running;

		int ret = -1;

		if (policy == SJF) {
				
				for (int i = 0; i < nproc; i++)	{
						if (proc[i].pid == -1 || proc[i].t_exec == 0)
								continue;
						if (ret == -1 || proc[i].t_exec < proc[ret].t_exec)
								ret = i;
				}

		} else if (policy == PSJF) {
				qsort(proc, nproc, sizeof(struct process), cmp_proc_sjf);
				
				for (int i = 0; i < nproc; i++)	{
						if (proc[i].pid == -1 || proc[i].t_exec == 0)
								continue;
						if (ret == -1 || proc[i].t_exec < proc[ret].t_exec)
								ret = i;
				}

		} else if (policy == FIFO) {

				for (int i = 0; i < nproc; i++)	{
						if (proc[i].pid == -1 || proc[i].t_exec == 0)
								continue;
						if (ret == -1 || proc[i].t_ready < proc[ret].t_ready)
								ret = i;
				}

		} else if (policy == RR) {
			
			if (running == -1)	{
					for (int i = 0; i < nproc; i++)	{
							if (proc[i].pid != -1 && proc[i].t_exec > 0) {
									ret = i;
									break;
							}
					}
			} else if ((ntime - t_last) % TIME_QUANTUM == 0) {
					ret = (running + 1) % nproc;
					
					while (proc[ret].pid == -1 || proc[ret].t_exec == 0)
							ret = (ret + 1) % nproc;
			} else
					ret = running;
		}

		return ret;
}

/** 
 * Schedule all the process
 * Check if process ready and execute,then chooese the next process,
 * run an unit of time and reapeat untill all the process are scheduled
 * @param proc is the set of process that are prepared to be scheduled
 * @param nporc is number of process
 * @param policy is the kind of scheduling algorithm, such as RR,SJF...
 * @return 0 if successful
*/
int scheduling(struct process *proc, int nproc, int policy)
{
		qsort(proc, nproc, sizeof(struct process), cmp_proc_fcfs);

		/* Initial pid = -1 imply not ready */
		for (int i = 0; i < nproc; i++)
				proc[i].pid = -1;

		/* Set single core prevent from preemption */
		proc_assign_cpu(getpid(), PARENT_CPU);

		/* Set high priority to scheduler */
		proc_wakeup(getpid());

		/* Initial scheduler */
		ntime = 0;
		running = -1;
		finish_cnt = 0;

		while (1) {
				//fprintf(stderr, "Current time: %d\n", ntime);

				/* Check if running process finish */
				if (running != -1 && proc[running].t_exec == 0)	{

						#ifdef DEBUG
									fprintf(stderr, "%s finish at time %d.\n", proc[running].name, ntime);
						#endif
						//kill(running, SIGKILL);
						waitpid(proc[running].pid, NULL, 0);
						printf("%s %d\n", proc[running].name, proc[running].pid);
						running = -1;
						finish_cnt++;

						/* All process finish */
						if (finish_cnt == nproc)
								break;
				}

				/* Check if process ready and execute */
				for (int i = 0; i < nproc; i++)	{
						if (proc[i].t_ready == ntime) {
								proc[i].pid = proc_exec(proc[i]);
								proc_block(proc[i].pid);
								#ifdef DEBUG
												fprintf(stderr, "%s ready at time %d.\n", proc[i].name, ntime);
								#endif
						}
				}

				/* Select next running  process */
				int next = next_process(proc, nproc, policy);

				if (next != -1)	{
					/* Context switch */
						if (running != next) {
								proc_wakeup(proc[next].pid);
								proc_block(proc[running].pid);
								running = next;
								t_last = ntime;
						}
				}

				/* Run an unit of time */
				UNIT_T();

				if (running != -1)
						proc[running].t_exec--;

				ntime++;
		}

		return 0;
}
