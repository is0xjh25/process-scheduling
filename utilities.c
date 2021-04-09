/*
COMP30023 Project One (utilities.c)
Created by Yun-Chi Hsiao (student ID: 1074004) 
Mar 20th, 2021
*/

#include "scheduler.h"
#include "utilities.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/******************************************************************************/
FILE* open_file(char *file_url) {

	FILE *file = NULL;
	file = fopen(file_url, "r");

	if (file == NULL) {
		printf("Error: File can't be read!\n");
	 	exit(-1);
	}

	return file;
}



void read_input(int argc, char **argv, char **file_url,
    int *num_processor, int *advanced_mode) {
    
    int option;
    
    while ((option = getopt(argc, argv, ":f:p:c")) != EOF){
      switch (option){
        case FILE_OPTION:
           *file_url = optarg;
           break;
        case PROCESS_OPTION:
            *num_processor = atoi(optarg);
            break;
        case advanced_OPTION:
            *advanced_mode = TRUE;
            break;
        case '?':  
            printf("Error: Unknown option in command line argument!\n");
            exit(-1); 
            break; 
      }
   }
}



void print_executing(sched *sch, proc *process) { 
    if (process->child_pid == NO_SUB_ID) {
        // parent process executing
        printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n",
            sch->current_time, process->pid, process->remaining_time, 
            process->cid);
    } else {
        // child process executing
        printf("%d,RUNNING,pid=%d.%d,remaining_time=%d,cpu=%d\n",
            sch->current_time, process->pid, process->child_pid,
            process->remaining_time, process->cid);
    }
}



void print_finished(sched *sch, proc *process) {
        printf("%d,FINISHED,pid=%d,proc_remaining=%d\n",
                sch->current_time, process->pid, sch->process_remaining); 
}



void print_performance(sched *sch) {
    
    int makespan = 0;
    double turn_around_self = 0;
    double turn_around_tot = 0;
    double turn_around_avg = 0;
    double time_overhead_tot = 0.0;
    double time_overhead_max = 0.0;
    double time_overhead_avg = 0.0;
    double time_overhead[sch->init_process_remaining];
    
    proc *current = sch->finished;
    proc *parent = NULL;
    proc *child = NULL;

    // find the lastest completed time for every parent
    while (TRUE) {
        // parent has children
        if (current->child_pid == NO_SUB_ID && current->child_num > 0) {
            
            int max_time_completed = 0;
            parent = current;
            child = sch->finished;
            
            while (child) {
                if (child->pid == parent->pid && 
                    child->time_compeleted > max_time_completed) {     
                    max_time_completed = child->time_compeleted;
                }
                child = child->link;
            }

            // record in parent
            parent->time_compeleted = max_time_completed;
        }
        
        if (!current->link) {
            break;
        } else {
            current = current->link;
        }
    }

    int i = 0;
    current = sch->finished;
    // calculate required performance statistics
    while (TRUE) {       
        
        // find parents
        if (current->child_pid == NO_SUB_ID) {
            turn_around_self = current->time_compeleted - current->time_arrived;
            turn_around_tot += turn_around_self;
            time_overhead[i] = turn_around_self / current->execution_time;
            // find when all process finished
            if (current->time_compeleted > makespan) {
                makespan = current->time_compeleted;
            }
            i++;
        }

        if (!current->link) {
            break;
        } else {
            current = current->link;
        }
    }
    
    // find each process time overhead
    for (i = 0; i < sch->init_process_remaining; i++) {    
        // record the max time overhead
        if (time_overhead[i] > time_overhead_max) {
            time_overhead_max = time_overhead[i];
        }
        time_overhead_tot += time_overhead[i];
    }
    
    // round numbers
    time_overhead_max = round(time_overhead_max*100)/100;
    time_overhead_avg = time_overhead_tot/sch->init_process_remaining;
    time_overhead_avg = round(time_overhead_avg*100)/100;
    turn_around_avg = turn_around_tot/sch->init_process_remaining;
    turn_around_avg = 
    round(turn_around_tot*100)/sch->init_process_remaining/100;
    
    printf("Turnaround time %0.f\n", ceil(turn_around_avg));
    printf("Time overhead %.3g %.3g\n", time_overhead_max, time_overhead_avg);
    printf("Makespan %d\n", makespan);
}