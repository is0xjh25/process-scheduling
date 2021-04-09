/*
COMP30023 Project One (scheduler.h)
Created by Yun-Chi Hsiao (student ID: 1074004)
Mar 20th, 2021

This is the header file for scheduler.c.
This contains the default algorithm(shortest_time_remaining, STR) which can work
with single, two and N processor scheduler. Also, it includes advanced algorithm
which would improve processor scheduler performance on makespan.
*/

#ifndef SCHEDULERH
#define SCHEDULERH
#define FILE_OPTION 'f'
#define PROCESS_OPTION 'p'
#define advanced_OPTION 'c'
#define SEPARATOR " "
#define PARALLELISABLE 'p'
#define NON_PARALLELISABLE 'n'
#define TRUE 1
#define FALSE 0
#define TUPLE_NUM 4
#define TIME_ARRIVED 1
#define PID 2
#define EXECUTION_TIME 3
#define IS_PARALLELISABLE 4
#define NO_SUB_ID -1
#define TIME_GAP 1
#define SINGLE 1
#define DUO 2

#include "utilities.h"
#include <stdio.h>

typedef struct process {
	struct process *link;
    int time_arrived;
    int execution_time;
    int remaining_time;
    int cid;
    int time_compeleted;
    int pid;
    int child_pid;
    int child_num;
    char is_parallelisable;   
} proc;

typedef struct scheduler {
    int current_time;
    int num_processor;
    int process_finished;
    int process_remaining;
    int init_process_remaining;
    struct process *pending;
    struct process *finished;
    struct process **processors;
} sched;

proc* read_process(char *line);
proc* create_child_process(proc *parent, int num_child, int time);
proc* link_process(proc *head, proc *process);
proc* arrived_insert(proc *head, proc *new_process);
proc* str_insert(proc *head, proc *new_process);
proc* advanced_insert(proc *head, proc *new_process);
proc** create_processors(int num_processor);
proc** store_processing(sched *scheduler, proc **temp);
sched* init_scheduler(FILE *file, int num_processor, int advanced_mode);
sched* check_finished(sched *scheduler);
int remaining_time(proc *running_list);
int default_k(sched *sch, proc *process);
int advanced_k(sched *sch, proc *process);
void run_process(sched *scheduler);
void check_new_processing(sched *scheduler, proc **temp);
void free_scheduler(sched *scheduler);
void default_processor_scheduler(FILE *file, int num_processor, 
                                int advanced_mode);
void advanced_processor_scheduler(FILE *file, int num_processor, 
                                int advanced_mode);

#endif