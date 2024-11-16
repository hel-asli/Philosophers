/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamza <hamza@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/16 12:23:54 by hamza            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
#include <string.h>

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

// void *routine(void *arg)
// {
// 	int n = *(int *)arg; 
// 	for (int i = 0; i < n; i++)
// 	{
// 		printf("hello world\n");
// 	}
// 	*(int *)arg = 42;
// 	int *a = malloc(sizeof(int));
// 	*a = 1337;
// 	fprintf(stderr, "tid : %lu\n", (unsigned long)pthread_self());

// 	return (a);
// }

// void *routinev2(void *arg)
// {
// 	pthread_t *t = arg;
// 	pthread_cancel(*t);
// 	fprintf(stderr, "%lu --> cancel from %lu\n", (unsigned long)(*t), (unsigned long)pthread_self());
// 	return (NULL);
// }

void *siham(void *arg)
{
	while (1)
	{
		printf("hamza hmar\n");
	}
	return (arg);
}

void *hamza(void *arg)
{
	while(1)
	{
		printf("ook\n");
	}
	return (arg);
}

// why i can change a local variable that declared in the main thread in another routine &&
// i can'teturn a pointer to var that is declared staticly ?
// 
bool is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int count_words(char *str)
{
	int i = 0;
	int w = 0;

	while (is_space(str[i]))
		i++;
	while (str[i])
	{
		if (!is_space(str[i]) && (!str[i + 1] || is_space(str[i + 1])))
			w++;
		i++;
	}
	return (w);
}

int check_str(char *str)
{
	int i = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (!str[i])
		return (1);
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9') && !is_space(str[i]))
		{
			return (1);
		}
		i++;
	}
	// "   +    "
	return (0);
}

int only_plus(char *str)
{
	size_t i;

	i = 0;
	while (str[i])
	{
		if (is_space(str[i]) || str[i] == '+')
			i++;
		else
			break;
	}

	if (i == strlen(str))
		return (1);
	return (0);
}


int check_args(char **av)
{
	int i = 0;

	while (av[i])
	{
		if (check_str(av[i]) || count_words(av[i]) > 1 || only_plus(av[i])) // only space
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	// pthread_t t[2];
	(void)av;
	(void)ac;
	if (ac < 5 || ac > 6)
	{
		fprintf(stderr, "%s\n", ERR_MSG);
		return (1);
	}

	if (check_args(++av))
	{
		fprintf(stderr, "Error: Arg not valid\n");
		return (1);
	}
	// pthread_create(&t[0], NULL, hamza, NULL);
	// pthread_create(&t[1], NULL, siham, NULL);
	// pthread_join(t[0], 	NULL); // this mean the wait for thread to finish before continue .  pthread_join(t[1], NULL);
	// pthread_join(t[1], NULL);
	// pthread_detach(t); // this means the thread runs independently and is not joined .
	
}
