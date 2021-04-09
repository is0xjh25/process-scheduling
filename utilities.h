/*
COMP30023 Project One (utilities.h)
Created by Yun-Chi Hsiao (student ID: 1074004) 
Mar 20th, 2021

This is the header file for utilites.c.
The following functions are aim to read command line, the number of processors,
the content in provided file and print out required transcipt.
If any of these process fails, the program would be terminated.
*/

#ifndef UTILITIESH
#define UTILITIESH
#include "scheduler.h"
#include <stdio.h>

typedef struct process proc;
typedef struct scheduler sched;

FILE* open_file(char *file_url);
void read_input(int argc, char **argv, char **file_url, int *num_processor, 
                int *optimal_mode);
void print_executing(sched *sch, proc *process);
void print_finished(sched *sch, proc *process);
void print_performance(sched *sch);

#endif