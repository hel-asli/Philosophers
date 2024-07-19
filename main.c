/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/18 05:32:12 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
# define NUM_THREADS 2

// the main function it self is a routine executed in the main thread . 
	/* <-- pthread_create() -->
	 * pthread_create is a func is used to create a thread with in a process  .
	 int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
	 thread ==> a pointer to a pthread_t var that will store the thread id . 
	 attr ==> A pointer to pthread_d attr that specify attr , can be NULL for default val. 
	 routine ==> a pointer to func that return (void *) and take (void *) as param .
	 arg ==> void pointer for arg to pass to routing func . 
	*/
	// the thread terminate in one of those three cases : 
	// use pthread_exit() 
	// return from routine 
	// cancel from another thread use pthread_cancel
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

// pthread (posix thread)let you create and manage threads 
// posix => portable operating system interface
// what is cores ? 
/**
 * the cpu (central processing uint) can execute millions of instruction per second - but one instruction at a time . --> with physical cores 
 * hyperthreading ==> a singal physical core appears as multiple physical cores , making the os think there is more cores than actualy exists useing (virtual cores)
 * 
**/

// google chrome each tab is a isolated process , web servers are use process to seperate each communication with childs . 
// but this approch is still time and resource consuming even with optimization thechnique 
// ---> the threads solve those two issues because of (shared memory || global memory (data segment and heap)) (cpu = centeral processing unit). 
// ---> threads creation is faster than process creation 10x . 
// ---> threds are created using the clone system call . many of attr that must be dup for child ps instead are shared between threads .
// ---> you can wait for a thread to continue execution of the passed function , this is called joining .
// ---> if you don't care when the thread completes and don't want to wait, you can detach it. 
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
// all threads in a single process have access to the same ps components , such as fds and memory .
// errno is a global variable int use a syscall to specifiy the error that let this syscall fail . 
// for thread the errno is unique for each thread . 
void err_exit(int err, char *str)
{ 
	fprintf(stderr, "%d\t%s", err, str);
	exit(EXIT_FAILURE);
}
void *print_hello(void *thread_id)
{
	long tid = (long)thread_id;

	printf("hello from thread %ld\n", tid);
	pthread_exit(NULL);
}

int n = 3;

void *print_num(void *arg)
{
	(void)arg;
	n = 42;	
	sleep(2);
	printf("*** : %d\n", n);
	return (NULL);
}

void *print_n(void *arg)
{
	(void)arg;
	sleep(2);
	printf("*** : %d\n", n);
	return (NULL);
}

void *run(void *arg)
{
	int *a = (int *)arg;
	printf("THREAD: runing thread with arg %d\n", *a);
	sleep(2);

	return (NULL);
}
void print_ids(const char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid\t %d tid\t%lu 0x%lx\n", s, pid, (unsigned long)tid, (unsigned long)tid);
}
void *thr_fun(void *arg)
{
	(void)arg;
	// pthread_exit(arg);
	print_ids("new thread : ");
	return ((void *)0);
}

void lek(void)
{
	system("leaks -c philo");
}

void voidd(int si)
{
	(void)si;
	system("leaks -c philo");
}
int	main(int ac, char **av)
{
	(void)ac;
	// what is pthread_t ? 
	pthread_t threads[2];
	void	*ret;
	int		err;
	char	*ptr;

	atexit(lek);
	for(int i = 0; i < 2; i++)
	{
		err = pthread_create(&threads[i], NULL, thr_fun, av[1]);
		if (err != 0)
			err_exit(err, "can't creaet thread");
	}
	for (int i = 0; i < 2; i++)
	{
		pthread_join(threads[i], &ret);
		printf("tid :\t%lu\treturn value of the thread_func is :\t%s\n", (unsigned long)threads[i], ret);
	}
	ptr = malloc(sizeof(char *) * 4);
	// when we use exit _exit || _Exit a signal is sent to a thread will terminate the entire process.
	// NULL
	// the sizeof pthread_t can vary between systems . 
	// it can be int or long or struct => we use pthread_equal to compare two thread ids . 
	// pthread_t is an opaque data ty
	// 0 if ok , err number on failure .
	signal(SIGQUIT, voidd);
	pthread_exit(NULL);
}
