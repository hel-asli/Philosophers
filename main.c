/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/14 03:33:32 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** <------ Processes ------>
 *  A process is a program under execution with own memory space (vm)
 *  Processes are isolated from each other and don't share memory directly
 *  Each process has its own Process Control Block (PCB) in os.
 * 	Inter-process communication (IPC) is relatively expensive : 
 *   1 -> IPC mechanisms allow processes to communicate and synchronize their action when needed .
 *   2 -> PIPES, 
**/
// A process can have multiple thread but the first called the main thread . 
// --- > single threaded process and multi threading process . 
// like process threads are used by apps to perform multiple tasks concurrently 
// the threads are tracked using a virtual address on the text and code segment 
// thread a segment of a process . 


// google chrome each tab is a isolated process , web servers are use process to seperate each communication with childs . 
// but this approch is still time and resource consuming even with optimization thechnique 
// ---> the threads solve those two issues because of (shared memory || global memory (data segment and heap))
// ---> threads creation is faster than process creation 10x . 
// ---> threds are created using the clone system call . many of attr that must be dup for child ps instead are shared between threads .
// * COW (copy on write) is not required for threads . 
/* ---- threads share ===> :
	* open file descriptors .
	* signal dispoitions . 
	* interval timers (setitimer(), timer_create())
	* resource limits . 
*/
/*
	--- threads unique attr
	* thread id
	* signal mask .
	* errno variable .
	* stack  
*/
// posix threads api .
// pthreads data_type : 
/*
 pthread_t => thread identififer
 ...
*/

// errno is a global variable int use a syscall to specifiy the error that let this syscall fail . 
// for thread the errno is unique for each thread . 
void err_exit(char *str)
{ 
	perror(str);
	exit(EXIT_FAILURE);
}

int n = 3;

void *print_num(void *arg)
{
	(void)arg;
	n = 42;	
	printf("*** : %d\n", n);
	return (NULL);
}

void *print_n(void *arg)
{
	(void)arg;
	printf("*** : %d\n", n);
	return (NULL);
}



int	main(void)
{
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, print_num, NULL);
	pthread_join(t1, NULL);
	pthread_create(&t2, NULL, print_n, NULL);
	pthread_join(t2, NULL);
}
