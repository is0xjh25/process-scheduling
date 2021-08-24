# Process-Scheduling Management

## Summary
•	Familiarized with process scheduling in a multi-processor environment\
•	Simulated and allocated CPU to the running processes\
•	Used various CPU Scheduling Algorithms to meet various requirements\

## Requirement

### 1.Single processor scheduler
Processes will be allocated CPU time according to shortest-time-remaining algorithm, a preemptive scheduling algorithm. When a new process arrives, 
it is scheduled if there is no other process running. If, when process j arrives, 
there is a process i running, then i is postponed and added to a queue if and only if j has a shorter execution time than the remaining time of i. 
Process i is resumed where it left off, if it is the process with the shortest remaining time left among all other processes in the queue 
(i.e., including those that may have arrived after j). 
Process ids are used to break ties, giving preference to processes with smaller ids. 
Since there is only one CPU, you can ignore the information on whether a process is parallelisable or not.

### 2.Two processor scheduler
In this section you are asked to extend your scheduler to work with 2 processors: processors numbered 0 and 1. This will simulate (1) a situation where processes can run truly in parallel and (2) a process that can run faster with more computational resources (e.g., by creating child/sub processes).

If the process that arrives is not parallelisable, it is assigned to the queue of the CPU with the smallest amount of remaining execution time for all processes and subprocesses (defined below) allocated to it, using CPU id to break ties, giving preference to CPU 0. After that the simulation for this process behaves same as in Section 1. In this project, once a process is assigned to a CPU it cannot be moved to another CPU. (Think of why it may not be a good idea to migrate processes between CPUs.)

If arriving process with id i and execution time x is parallelisable, two subprocesses are created, i.0 and i.1, each with execution time ⌈x/2⌉ + 1. The extra time simulates the cost of synchronisation. For example, a parallelisable process that runs on a single CPU in 6 seconds, can finish within 4 seconds if both CPUs are idle when it arrives.

Once subprocesses are created, subprocess i.0 is added to the queue of CPU 0 and i.1 is added to CPU 1. After that they are treated as regular processes when scheduling them on each CPU. A parallelisable process is considered finished when both of its subprocesses have finished.

### 3.N processor scheduler
In this task we generalise the 2 processor setting to N ≥ 3 processors. Similar to above, a non- parallelisable process is assigned to a CPU that has the shortest remaining time to complete all processes and subprocesses assigned to it so far.

A parallelisable process is split into k ≤ N subproceses, where k is the largest value for which x/k ≥ 1. Each subprocess is then assigned execution time of ⌈x/k⌉ + 1. Subprocesses follow a similar naming convention as above: a process with id i is split into subprocesses with id’s i.0, i.1, ..., i.k′, where k′ = k − 1. Subprocesses are then assigned to k processors with shortest time left to complete such that subprocess i.0 is assigned to the CPU that can finish its processes the fastest, i.1 is assigned to the second fastest processor and so on. Use CPU ids to break ties, giving preference to smaller processor numbers.

Once a process or subprocess is assigned to a CPU it cannot migrate to another CPU. A parallelisable process is considered finished when all of its subprocesses have finished.

### 4.Challenge: Improve the performance
You will be asked to measure the overall time of your simulation (Makespan). The challenge task is to come up with an algorithm that has a shorter makespan on a set of tests (see details in Section 7). For this task the choice of k when splitting a parallelisable process is left to you. You are also allowed to “look into the future” and see what processes will be arriving and use this information if you choose to. (Think of whether it is possible and how one would obtain such information in real life.) You will be required to explain why your algorithm is more efficient in a short report.

---
_more details in project specification!_
