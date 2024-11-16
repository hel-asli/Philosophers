#include "philo.h"
# define NUMS 4
#include <stdatomic.h>


atomic_int cnt = 0;
pthread_mutex_t lock;
int flag = 0;

void *count()
{
	int i = 0;

	while (i++ < 1000000)
	{
		pthread_mutex_lock(&lock);
		cnt++;
		pthread_mutex_unlock(&lock);
	}

	return (NULL);
}

// thread share => data segment, text code segment, head, fds table, ( interval timers ? )
// thread unique => stack, tid is uniqe on ps , errno ==> (LTS) , ( ps register ? )

// context switching of threads is faster than ps context switching

int main(void)
{
	pthread_t t[NUMS];
	pthread_mutex_t lock;
	if (pthread_mutex_init(&lock, NULL))
		return (1);

	for (int i = 0; i < NUMS; i++)
	{
		if (pthread_create(t + i, NULL, count, NULL))
			return (1);
		printf("%d thread created\n", i);
	}

	for (int i = 0; i < NUMS; i++)
	{
		if (pthread_join(t[i], NULL))
			return (1);
		printf("%d thread executed\n", i);
	}
	pthread_mutex_destroy(&lock);
	printf("cnt: %d\n", cnt);
}