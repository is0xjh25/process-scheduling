# Process-Scheduling Management (C Language):

## Summary
•	Familiarized with process scheduling in a multi-processor environment\
•	Simulated and allocated CPU to the running processes\
•	Used various CPU Scheduling Algorithms to meet various requirements\

## Requirments

# 1.Single processor scheduler
Processes will be allocated CPU time according to shortest-time-remaining algorithm, a preemptive scheduling algorithm. When a new process arrives, 
it is scheduled if there is no other process running. If, when process j arrives, 
there is a process i running, then i is postponed and added to a queue if and only if j has a shorter execution time than the remaining time of i. 
Process i is resumed where it left off, if it is the process with the shortest remaining time left among all other processes in the queue 
(i.e., including those that may have arrived after j). 
Process ids are used to break ties, giving preference to processes with smaller ids. 
Since there is only one CPU, you can ignore the information on whether a process is parallelisable or not.
