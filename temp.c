/*
COMP30023 Project One (sch.c)
Created by Yun-Chi Hsiao (student ID: 1074004) Mar 20th, 2021
*/

#include "scheduler.h"
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/
proc* read_process(char *line) {
    
	proc *process = (proc*)calloc(1, sizeof(proc));
	assert(process);

    // default setting
    process->link = NULL;
	process->time_arrived = 0;
    process->cid = 0;
	process->pid = 0;
    process->child_pid = NO_SUB_ID;
    process->child_num = 0;

    char *token = strtok(line, SEPARATOR);
    int count = 0;

    while (token != NULL) {
      
      count++;

      switch (count) {
        case TIME_ARRIVED:
            if (atoi(token) >= 0) {
                process->time_arrived = atoi(token);
            } else {
                printf("Error: Invalid time arrived!\n");
                exit(-1);
            }                
            break;

        case PID:
            if (atoi(token) >= 0) {
                process->pid = atoi(token);
            } else {
                printf("Error: Invalid processor ID!\n");
                exit(-1);
            }    
            break;

        case EXECUTION_TIME:
            if (atoi(token) > 0) {
                process->execution_time = atoi(token);
                process->remaining_time = atoi(token);
            } else {
                printf("Error: Invalid execution time!\n");
                exit(-1);
            }
            break;

        case IS_PARALLELISABLE:
            if ((*token == PARALLELISABLE) || (*token == NON_PARALLELISABLE)) {
                process->is_parallelisable = *token;
            } else {
                printf("Error: Invalid character!\n");
                exit(-1);
            }
            break;

        default: 
            printf("Error: Too many tuples in one line!\n");
			exit(-1);
			break;
        }

        token = strtok(NULL, SEPARATOR);
    }

    return process;
}



proc* create_child_process(proc *parent, int num_child, int time) {

    parent->child_num = num_child;

    for (int i = 0; i < num_child; i++) {
        
        proc *child = (proc*)calloc(num_child, sizeof(proc));
	    assert(child);

        // set children detail
        parent = link_process(parent, child);
        child->time_arrived = parent->time_arrived;
        child->execution_time = parent->execution_time;
        child->cid = 0;
        child->time_compeleted = parent->time_compeleted;
        child->pid = parent->pid;
        child->child_pid = i;
        child->child_num = 0;
        child->is_parallelisable = NON_PARALLELISABLE;
        child->execution_time = time;
        child->remaining_time = child->execution_time;
    }

    return parent;
}



proc* link_process(proc *head, proc *process) {
    
    proc *temp = head;
    
    if (!temp) {
        head = process;
    } else if (temp) {    
        while (temp->link) {
            temp = temp->link;
        }
        temp->link = process;
    }

    return head; 
}



proc* pending_insert(proc *head, proc *new_process) {

    if (!head) {  
        head = new_process;
        return head;
    } 
    
    proc* current = head;
    proc* previous = NULL;

    while (current) {    
        
        if (new_process->time_arrived < current->time_arrived 
            || (new_process->time_arrived == current->time_arrived
            && new_process->execution_time < current->execution_time)
            ||(new_process->time_arrived == current->time_arrived
            && new_process->execution_time == current->execution_time
            && new_process->pid < current->pid)) {
            
            if (!previous) {
                new_process->link = head;
                head = new_process;
            } else if (previous) {
                previous->link = new_process;
                new_process->link = current;
            }
            return head;
        } else {
            if (current->link) {
                previous = current;
                current = current->link;
            } else if (!current->link) {
                current->link = new_process;            
                return head;
            } 
        }
    }

    return head;
}



proc* optimal_pending_insert(proc *head, proc *new_process) {

    if (!head) {  
        head = new_process;
        return head;
    } 
    
    proc* current = head;
    proc* previous = NULL;

    while (current) {    
        
        if (new_process->execution_time > current->execution_time 
            || (new_process->execution_time == current->execution_time
            && new_process->time_arrived < current->time_arrived)
            ||(new_process->execution_time == current->execution_time
            && new_process->time_arrived == current->time_arrived
            && new_process->pid < current->pid)) {
            
            if (!previous) {
                new_process->link = head;
                head = new_process;
            } else if (previous) {
                previous->link = new_process;
                new_process->link = current;
            }
            return head;
        } else {
            if (current->link) {
                previous = current;
                current = current->link;
            } else if (!current->link) {
                current->link = new_process;            
                return head;
            } 
        }
    }

    return head;
}



proc* str_insert(proc *head, proc *new_process) {

    if (!head) {  
        head = new_process;
        return head;
    } 
    
    proc* current = head;
    proc* previous = NULL;

    while (current) {    
        
        if (new_process->remaining_time < current->remaining_time 
            || (new_process->remaining_time == current->remaining_time 
            && new_process->pid < current->pid)) {
            
            if (!previous) {
                new_process->link = head;
                head = new_process;
            } else if (previous) {
                previous->link = new_process;
                new_process->link = current;
            }
            return head;
        } else {
            if (current->link) {
                previous = current;
                current = current->link;
            } else if (!current->link) {
                current->link = new_process;            
                return head;
            } 
        }
    }

    return head;
}



proc** create_processors(int num_processor) {
    
    proc **processors = (proc**)calloc(num_processor, sizeof(proc*));
    assert(processors);

    for (int i = 0; i < num_processor; i++) {
        processors[i] = NULL;
    }

    return processors;
}



proc** store_current_processing(sched *sch, proc **temp) { 
    for (int i = 0; i < sch->num_processor; i++) {
        temp[i] = sch->processors[i];
    }
    return temp;  
}



sched* init_scheduler(FILE *file, int num_processor, int optimal_mode) {

    sched *sch = (sched*)calloc(1, sizeof(sched));
    assert(sch);

    // default setting
    sch->num_processor = num_processor;
    sch->current_time = 0;
    sch->process_finished = 0;
    sch->process_remaining = 0;
    sch->init_process_remaining = 0;
	sch->pending = NULL;
	sch->finished = NULL;
    sch->processors = create_processors(num_processor);
    
    // read line (process)
    char *line = NULL;
    proc *process = NULL;
    size_t line_buffer_length = 0;
	
    while (getline(&line, &line_buffer_length, file) != EOF) {
	    process = read_process(line);
        if (optimal_mode == FALSE) {
		    sch->pending = pending_insert(sch->pending, process);
        } else if (optimal_mode == TRUE) {
            sch->pending = optimal_pending_insert(sch->pending, process);
        }
        (sch->init_process_remaining)++;
	}

    free(line);

    return sch;
}



sched* check_finished(sched *sch) {
    
    int is_finished = FALSE;
    int is_completely_finished = FALSE;
    int finished_count = 0;
    int child_needed = 0;
    int child_count = 0;
    proc* result[sch->process_remaining];

    for (int i = 0; i < sch->num_processor; i++) {
        
        proc *temp = sch->finished;
        
        if (sch->processors[i] 
            && sch->processors[i]->remaining_time == 0 
            && sch->processors[i]->child_pid == NO_SUB_ID 
            && sch->processors[i]->child_num == 0) {
            
            // is parent without child & finish
            is_finished = TRUE;
            is_completely_finished = TRUE;

        } else if (sch->processors[i] && 
            sch->processors[i]->remaining_time == 0) {  
            
            // is parent without child & is finished
            is_finished = TRUE;
            
            while (temp) {
                if (temp->pid == sch->processors[i]->pid 
                    && temp->child_pid == NO_SUB_ID) {
                    // parent
                    child_needed = temp->child_num;
                } else if (temp->pid == sch->processors[i]->pid 
                            && temp->child_pid != NO_SUB_ID)  {
                    // siblings
                    child_count++;
                }
                temp = temp->link;
            }

            // since current process is not finished_list yet, child_needed - 1
            if (child_count == child_needed - 1) {
                is_completely_finished = TRUE;
            } else {
                is_completely_finished = FALSE;
            } 
        }
        
        if (is_finished) {
            if (is_completely_finished) {
                // only when a process is completely finished
                finished_count++;
                (sch->process_remaining)--;
                (sch->process_finished)++;
                sch->processors[i]->time_compeleted = sch->current_time;               
                result[finished_count-1] = sch->processors[i];
            }
            // update finished list
            temp = sch->processors[i]->link;    
            sch->processors[i]->link = NULL;
            sch->finished = link_process(sch->finished, sch->processors[i]);
            sch->processors[i] = temp;
        } 

        // reset
        is_completely_finished = FALSE;
        is_finished = FALSE;
        child_needed = 0;
        child_count = 0;
    }
    
    // print out all finished transcipt
    for (int i = 0; i < finished_count; i++) {
        print_finished(sch, result[i]);
    }

    return sch;   
}



int remaining_time(proc *running_list) {
    
    int total_time = 0;
    proc *temp = running_list;
    
    if (!temp) {
        return total_time;
    }
    
    while(temp) {
        total_time += (temp->remaining_time);
        temp = temp->link;
    }

    return total_time;
}



int default_k(sched *sch, proc *process) {
    
    int k = 0;
    
    if (process->execution_time <= sch->num_processor) {
        k = process->execution_time;
    } else {
        k = sch->num_processor;
    }

    return k;
}



void run_process(sched *sch) {
    for (int i = 0; i < sch->num_processor; i++) {
        if (sch->processors[i]) {
            sch->processors[i]->remaining_time = 
            sch->processors[i]->remaining_time - TIME_GAP;
        }
    }
}



void check_new_processing(sched *sch, proc **temp) {
    for (int i = 0; i < sch->num_processor; i++) {
        if (sch->processors[i] && sch->processors[i] != temp[i]) {
            print_executing(sch, sch->processors[i]);
        }
    }
}



void free_scheduler(sched *sch) {
    
    proc *current = sch->finished;
    proc *next = NULL;
	
	while (current) {
        next = current->link;
		free(current);
		current = next;
    }

	free(sch->processors);
    free(sch);
}



void default_processor_scheduler(FILE *file, int num_processor, 
    int optimal_mode) {
    
    sched *sch = init_scheduler(file, num_processor, optimal_mode);
    proc **temp = create_processors(num_processor);

    while (sch->init_process_remaining > sch->process_finished) {

        temp = store_current_processing(sch, temp);   
        proc *previous = NULL;
        proc *current = sch->pending;
        proc *next = NULL;
        
        // depending on num_processor, the scheduler acts different
        switch (num_processor) {
            // one processor
            case SINGLE:
                while (current) {
                    next = current->link;
                    if (current->time_arrived == sch->current_time) {
                        (sch->process_remaining)++;
                        if (!previous) {
                            current->link = NULL;
                            sch->processors[SINGLE-1] = 
                            str_insert(sch->processors[SINGLE-1], current);
                            current = next;
                            if (next) {
                                sch->pending = current;
                            } else {
                                sch->pending = NULL;
                            }
                        }
                    } else {
                        previous = current;
                        current = next;            
                    }
                }
                break;

            // two processors
            case DUO:
                while (current) {
                    next = current->link;
                    if (current->time_arrived == sch->current_time) {       
                        (sch->process_remaining)++;
                        if (!previous) {
                            current->link = NULL;
                            if (current->is_parallelisable 
                                == NON_PARALLELISABLE) {
                                if (remaining_time(sch->processors[DUO-2])
                                    <= remaining_time(sch->processors[DUO-1])) {
                                    current->cid = DUO-2;
                                    sch->processors[DUO-2] =  
                                    str_insert(sch->processors[DUO-2], current);
                                } else {
                                    current->cid = DUO-1;
                                    sch->processors[DUO-1] = 
                                    str_insert(sch->processors[DUO-1], current);
                                }
                            } else if (current->is_parallelisable 
                                        == PARALLELISABLE) {
                                int time = 
                                ceil((double)current->execution_time/DUO)+1;
                                proc *parent = 
                                create_child_process(current, DUO, time);
                                proc *child_1 = parent->link;
                                proc *child_2 = child_1->link;  
                                // put parent into finished list;
                                parent->link = NULL;
                                sch->finished = 
                                link_process(sch->finished,parent);
                                // put children into two processors
                                child_1->link = NULL;
                                child_1->cid = DUO-2;
                                sch->processors[DUO-2] = 
                                str_insert(sch->processors[DUO-2], child_1);
                                child_2->link = NULL;
                                child_2->cid = DUO-1;
                                sch->processors[DUO-1] = 
                                str_insert(sch->processors[DUO-1], child_2);                       
                            }

                            current = next;

                            if (next) {
                                sch->pending = current;
                            } else {
                                sch->pending = NULL;
                            }
                        }
                    } else {
                        previous = current;
                        current = next;            
                    }
                }
                break;
            
            // n processors
            default:    
                while (current) {
                    next = current->link;
                    if (current->time_arrived == sch->current_time) {       
                        (sch->process_remaining)++;
                        if (!previous) {
                            current->link = NULL;
                            
                            if (current->is_parallelisable == 
                                NON_PARALLELISABLE) {
                                
                                int min_time = INT_MAX;
                                int index = 0;

                                for (int i = 0; i < num_processor; i++) {
                                    if (remaining_time(sch->processors[i])
                                        < min_time) {
                                        min_time
                                        = remaining_time(sch->processors[i]);
                                        index = i;
                                    }
                                }

                                current->cid = index;
                                sch->processors[index] = 
                                str_insert(sch->processors[index], current);

                            } else if (current->is_parallelisable 
                                        == PARALLELISABLE) {
                    
                                int k = default_k(sch, current);            
                                int time =
                                ceil((double)current->execution_time/k+1);
                                
                                int children[num_processor];
                                
                                for (int i = 0; i < num_processor; i++) {
                                    children[i] = FALSE;
                                }
                                
                                proc *parent = 
                                create_child_process(current, k, time);                               
                                proc *child = parent->link;
                                proc *child_temp = NULL;

                                for (int i = 0; i < k; i++) {
                                    
                                    int min_time = INT_MAX;
                                    int index = 0;
                                
                                    for (int i = 0; i < num_processor; i++) {
                                        if (remaining_time(sch->processors[i])
                                            < min_time && !children[i]) {
                                            min_time
                                            = remaining_time(sch->
                                            processors[i]);
                                            index = i;
                                        }
                                    }

                                    child_temp = child->link;
                                    child->cid = index;
                                    child->link = NULL;
                                    sch->processors[index] =
                                    str_insert(sch->processors[index], child);
                                    child = child_temp;
                                }

                                parent->link = NULL;
                                sch->finished = 
                                link_process(sch->finished,parent);
                            }

                            current = next;

                            if (next) {
                                sch->pending = current;
                            } else {
                                sch->pending = NULL;
                            }
                        }
                    } else {
                        previous = current;
                        current = next;            
                    }
                }
                break;
            }

        // check wethear process is finished
        sch = check_finished(sch);
        // check new process is going to execute
        check_new_processing(sch, temp);
        // simulate one gap(1sec)
        run_process(sch);
        // time tick
        sch->current_time += TIME_GAP; 
    }

    print_performance(sch);
	free_scheduler(sch);
	free(temp);
}




void optimal_processor_scheduler(FILE *file, int num_processor, 
                                int optimal_mode) {
    
    sched *sch = init_scheduler(file, num_processor, optimal_mode);
    proc **temp = create_processors(num_processor);
    // sch->init_process_remaining > sch->process_finished
    while (sch->init_process_remaining > sch->process_finished) {
        
        temp = store_current_processing(sch, temp);   
        
        proc *previous = NULL;
        proc *current = sch->pending;
        proc *next = NULL;
        
        // depending on num_processor, the scheduler acts different
        switch (num_processor) {
            // one processor
            case SINGLE:
                while (current) {
                    next = current->link;
                    if (current->time_arrived == sch->current_time) {
                        (sch->process_remaining)++;
                        if (!previous) {
                            current->link = NULL;
                            sch->processors[SINGLE-1] = 
                            str_insert(sch->processors[SINGLE-1], current);
                            current = next;
                            if (next) {
                                sch->pending = current;
                            } else {
                                sch->pending = NULL;
                            }
                        }
                    } else {
                        previous = current;
                        current = next;            
                    }
                }
                break;

            // two processors
            case DUO:
                while (current) {
                    next = current->link;  
                    if (current->time_arrived <= sch->current_time) {       
                        if (current->is_parallelisable == NON_PARALLELISABLE) {
                            if (!sch->processors[DUO-2]) {
                                (sch->process_remaining)++;
                                if (!previous) {
                                    current->link = NULL;
                                    current->cid = DUO-2;
                                    sch->processors[DUO-2] = 
                                    link_process(sch->processors[DUO-2], 
                                                current);
                                    current = next;
                                    if (next) {
                                        sch->pending = current;
                                    } else {
                                        sch->pending = NULL;
                                    }
                                } else {
                                    previous->link = next;
                                    current->link = NULL;
                                    current->cid = DUO-2;
                                    sch->processors[DUO-2] = 
                                    link_process(sch->processors[DUO-2], 
                                                current);
                                    current = next;
                                } 
                            } else if (!sch->processors[DUO-1]) {
                                (sch->process_remaining)++;
                                if (!previous) {
                                    current->link = NULL;
                                    current->cid = DUO-1;
                                    sch->processors[DUO-1] = 
                                    link_process(sch->processors[DUO-1], 
                                                current);
                                    current = next;
                                    if (next) {
                                        sch->pending = current;
                                    } else {
                                        sch->pending = NULL;
                                    }
                                } else {
                                    previous->link = next;
                                    current->link = NULL;
                                    current->cid = DUO-1;
                                    sch->processors[DUO-1] = 
                                    link_process(sch->processors[DUO-1], 
                                                current);
                                    current = next;
                                }
                            } else {
                                previous = current;
                                current = next;            
                            }
                        } else if (current->is_parallelisable 
                            == PARALLELISABLE) {
                            (sch->process_remaining)++;
                            if (!previous) {
                                current->link = NULL;
                                int time = 
                                ceil((double)current->execution_time/DUO)+1;
                                proc *parent = 
                                create_child_process(current, DUO, time);
                                proc *child_1 = parent->link;
                                proc *child_2 = child_1->link;  
                                // put parent into finished list;
                                parent->link = NULL;
                                sch->finished = 
                                link_process(sch->finished,parent);
                                // put children into two processors
                                child_1->link = NULL;
                                child_1->cid = DUO-2;
                                sch->processors[DUO-2] = 
                                optimal_pending_insert(sch->processors[DUO-2], child_1);
                                child_2->link = NULL;
                                child_2->cid = DUO-1;
                                sch->processors[DUO-1] = 
                                optimal_pending_insert(sch->processors[DUO-1], child_2); 
                                current = next;
                                if (next) {
                                    sch->pending = current;
                                } else {
                                    sch->pending = NULL;
                                }
                            } else if (previous) {
                                current->link = NULL;
                                int time = 
                                ceil((double)current->execution_time/DUO)+1;
                                proc *parent = 
                                create_child_process(current, DUO, time);
                                proc *child_1 = parent->link;
                                proc *child_2 = child_1->link;  
                                // put parent into finished list;
                                parent->link = NULL;
                                sch->finished = 
                                link_process(sch->finished,parent);
                                // put children into two processors
                                child_1->link = NULL;
                                child_1->cid = DUO-2;
                                sch->processors[DUO-2] = 
                                optimal_pending_insert(sch->processors[DUO-2], child_1);
                                child_2->link = NULL;
                                child_2->cid = DUO-1;
                                sch->processors[DUO-1] = 
                                optimal_pending_insert(sch->processors[DUO-1], child_2); 
                                previous->link = next;
                                current = next;
                            }                                
                        }
                    } else {
                        previous = current;
                        current = next; 
                    } 
                }
                break;
            
            // n processors
            default:    
                while (current) {
                    next = current->link;
                    if (current->time_arrived == sch->current_time) {       
                        (sch->process_remaining)++;
                        if (!previous) {
                            current->link = NULL;
                            
                            if (current->is_parallelisable == 
                                NON_PARALLELISABLE) {
                                
                                int min_time = INT_MAX;
                                int index = 0;

                                for (int i = 0; i < num_processor; i++) {
                                    if (remaining_time(sch->processors[i])
                                        < min_time) {
                                        min_time
                                        = remaining_time(sch->processors[i]);
                                        index = i;
                                    }
                                }

                                current->cid = index;
                                sch->processors[index] = 
                                str_insert(sch->processors[index], current);

                            } else if (current->is_parallelisable 
                                        == PARALLELISABLE) {
                                
                                int k = default_k(sch, current);
                                
                                int time =
                                ceil((double)current->execution_time/k+1);
                                
                                int children[num_processor];
                                
                                for (int i = 0; i < num_processor; i++) {
                                    children[i] = FALSE;
                                }
                                
                                proc *parent = 
                                create_child_process(current, k, time);
                                
                                proc *child = parent->link;
                                proc *child_temp = NULL;

                                for (int i = 0; i < k; i++) {
                                    int min_time = INT_MAX;
                                    int index = 0;
                                
                                    for (int i = 0; i < num_processor; i++) {
                                        if (remaining_time(sch->processors[i])
                                            < min_time && !children[i]) {
                                            min_time
                                            = remaining_time(sch->
                                            processors[i]);
                                            index = i;
                                        }
                                    }
                                    child_temp = child->link;
                                    child->cid = index;
                                    child->link = NULL;
                                    sch->processors[index] =
                                    str_insert(sch->processors[index], child);
                                    child = child_temp;
                                }
                                parent->link = NULL;
                                sch->finished = 
                                link_process(sch->finished,parent);
                            }

                        }
                    } else {
                        previous = current;
                        current = next;            
                    }
                }
                break;
            }
        
        // check wethear process is finished
        sch = check_finished(sch);
        // check new process is going to execute
        check_new_processing(sch, temp);
        // simulate one gap(1sec)
        run_process(sch);
        // time tick
        sch->current_time += TIME_GAP; 
    }

    print_performance(sch);
	free_scheduler(sch);
	free(temp);
}



