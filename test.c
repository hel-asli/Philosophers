#include "Philo/philo.h"
#include <string.h>
#include <sys/errno.h>
#include <sys/time.h>
# define STACK_SIZE 1000000
#include <stdatomic.h>



// thread share => data segment, text code segment, head, fds table, ( interval timers ? )
// thread unique => stack, tid is uniqe on ps , errno ==> (LTS) , ( ps register ? )

// context switching of threads is faster than ps context switching

// __thread  (LTS)

void *rt(void *arg)
{
	printf("hello world");
	return (arg);
}




int main(void)
{
	pthread_t t;
	pthread_attr_t attr; // pthread_attr_t is a struct witch is opaque . 
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&t, &attr, rt, NULL)) // fork point .
		return (1);

	pthread_exit(NULL);
	// waits for thread to complete execution and take there exit status .
	// and then the kernel can clean the resource allocated for that thread . 
	return (0);
}