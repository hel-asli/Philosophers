# Understanding Processes and Threads in POSIX Systems

## Table of Contents
- [Processes Overview](#processes-overview)
- [Threads Overview](#threads-overview)
- [CPU and Cores](#cpu-and-cores)
- [Process vs Threads](#process-vs-threads)
- [POSIX Threads (pthreads)](#posix-threads-pthreads)
- [Thread Creation and Management](#thread-creation-and-management)
- [Memory and Resource Sharing](#memory-and-resource-sharing)

## Processes Overview
A process is a program under execution with its own memory space (virtual memory). Key characteristics include:
- Isolation from other processes
- Own Process Control Block (PCB) in the operating system
- Inter-process communication (IPC) mechanisms:
  - Relatively expensive
  - Allows processes to communicate and synchronize actions
  - Examples include PIPES

## Threads Overview
- A thread is a segment of a process
- Processes can be single-threaded or multi-threaded
- The first thread in a process is called the main thread
- Threads enable applications to perform multiple tasks concurrently
- Threads are tracked using virtual addresses in text and code segments

## CPU and Cores
- CPU (Central Processing Unit) executes millions of instructions per second
- Physical cores: Real processing units on the CPU
- Hyperthreading:
  - Makes a single physical core appear as multiple cores
  - Creates virtual cores
  - Operating system sees more cores than physically exist

## Process vs Threads
### Advantages of Threads
- Faster creation (10x faster than process creation)
- Shared memory (global memory, data segment, and heap)
- No need for Copy-On-Write (COW)
- Created using the clone system call
- Lower resource consumption

### Thread Management
- Thread joining: Wait for thread completion
- Thread detaching: Allow independent execution

## POSIX Threads (pthreads)
POSIX (Portable Operating System Interface) threads provide standardized thread management.

### pthread_create Function
```c
int pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg
);
```
Parameters:
- `thread`: Pointer to pthread_t variable (stores thread ID)
- `attr`: Thread attributes (NULL for defaults)
- `start_routine`: Pointer to thread function
- `arg`: Arguments for thread function

### Thread Termination
Threads can terminate in three ways:
1. Using pthread_exit()
2. Returning from routine
3. Cancellation by another thread using pthread_cancel

## Memory and Resource Sharing

### Shared Components Between Threads
- Open file descriptors
- Signal dispositions
- Interval timers (setitimer(), timer_create())
- Resource limits
- Process memory (data segment and heap)

### Thread-Unique Attributes
- Thread ID
- Signal mask
- errno variable (unique per thread)
- Stack

## Real-World Examples
- Google Chrome: Each tab runs as an isolated process
- Web servers: Use processes to separate client communications
- Error handling: Each thread maintains its own errno for system call failures

## Notes
- Main function executes in the main thread
- POSIX threads API provides standardized thread management
- Threads share process components while maintaining unique identifiers
- Thread-based approaches solve many resource and time consumption issues present in process-based solutions