#include "philo.h"
#include <string.h>
#include <sys/errno.h>
#include <sys/time.h>
# define NUMS 1
#include <stdatomic.h>



// thread share => data segment, text code segment, head, fds table, ( interval timers ? )
// thread unique => stack, tid is uniqe on ps , errno ==> (LTS) , ( ps register ? )

// context switching of threads is faster than ps context switching

// __thread 


pthread_mutex_t lock; 
pthread_mutex_t water_lock; 

int coffe = 1;
int water = 2;

#define YELLOW "\e[1;33m"
#define WHITE "\e[1;37m"

void *routine(void *arg)
{
	printf("thread - %d\n", coffe);
	pthread_mutex_lock(&lock);
	coffe++;
	pthread_mutex_unlock(&lock);
	printf("thread - %d\n", coffe);
	return (NULL);
}

size_t get_current_time(void)
{
	struct timeval ctime; 


	if (gettimeofday(&ctime, NULL)) // why we pass NULL ? 
		return (0);
	
	return ((ctime.tv_sec * 1000) +  (ctime.tv_usec / 1000));
	
}




int main(int ac, char *av[])
{
	pthread_t t[NUMS];
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&water_lock, NULL);


	for (int i = 0; i < NUMS; i++)
	{
		pthread_create(&t[i], NULL, routine, NULL);
	}

	for (int i = 0; i < NUMS; i++)
	{
		pthread_join(t[i], NULL);
	}
	printf("number of coffer %d\n", coffe);
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&water_lock);
	return (0);
}
