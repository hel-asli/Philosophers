/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/18 23:28:26 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
#include <string.h>



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


// why i can change a local variable that declared in the main thread in another routine &&
// i can'teturn a pointer to var that is declared staticly ?
// 

void *philo_routine(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	printf("hello from philo number :%d\n", data->philo->philo_id);
	return (arg);
}
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

size_t ft_atoi(char *str)
{
	int i = 0;
	size_t r;

	r = 0;
	while (str[i] && is_space(str[i]))
		i++;	
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - 48) ;
		i++;
	}
	return (r);
}

int check_str(char *str)
{
	int i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
	{
		fprintf(stderr, "Error: Arg not valid\n");
		return (1);
	}
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			fprintf(stderr, "Error: Arg not valid\n");
			return (1);
		}
		i++;
	}
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

int check_digits(char *str)
{
	int i;
	int	nb_digits;

	i = 0;

	nb_digits = 0;
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] == '0')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		nb_digits++;
		i++;
	}

	if (nb_digits >= 12)
	{
		fprintf(stderr, "number to big\n");
		return (1);
	}
	return (0);
}


int check_args(char **av)
{
	int i = 0;

	while (av[i])
	{
		if (check_str(av[i]) || check_digits(av[i]))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int data_init(t_data *data, char **av, int ac)
{
	data->nb_philos = ft_atoi(av[0]);
	if (!data->nb_philos || data->nb_philos > 200)
	{
		fprintf(stderr, "number_of_philosophers not valid\n");
		data->exit_status = 1;
		return (0);
	}
	data->time_die = ft_atoi(av[1]);
	data->time_eat = ft_atoi(av[2]);
	data->time_sleep = ft_atoi(av[3]);
	if (data->time_die < 60 || data->time_eat < 60 || data->time_sleep < 60)
	{
		fprintf(stderr, "time must be >= 60");
		data->exit_status = 1;
		return (0);
	}
	if (ac == 6)
	{
		data->nb_must_eat = ft_atoi(av[4]);
		if (!data->nb_must_eat)
		{
			data->exit_status = 1;
			fprintf(stderr, "number_of_times_each_philosopher_must_eat not valid\n");
			return (0);
		}
	}
	else
		data->nb_must_eat = 0;

	return (1);
}

void destory_mutex(t_data *data, size_t i)
{
	size_t	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&data->forks[j]);
		j++;
	}
}

int fork_mutex_init(t_data *data)
{
	size_t				i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			destory_mutex(data, i);
			free(data->forks);
			return (1);
		}
		i++;
	}
	return (0);
}
int philo_create(t_data *data)
{
	size_t i;

	i = 0;
	data->philo = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philo)
	{
		destory_mutex(data, data->nb_philos);
		free(data->forks);
		return (1);
	}
	while (i < data->nb_philos)
	{
		data->philo->philo_id = i + 1;
		data->philo->left_fork = &data->forks[i];
		data->philo->right_fork = &data->forks[(i + 1) % (data->nb_philos)];
		if (pthread_create(&data->philo[i].tid, NULL, philo_routine, data))
			return (1);
		i++;
	}
	for (size_t i = 0; i < data->nb_philos; i++)
	{
		if (pthread_join(data->philo[i].tid, NULL))
		{
			destory_mutex(data, data->nb_philos);
			free(data->forks);
			free(data->philo);
			return (1);
		}
	}
	return (0);
}


int	pthread_init(t_data *data)
{
	if (fork_mutex_init(data))
		return (1);
	if (philo_create(data))
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data data;

	memset(&data, 0, sizeof(t_data));
	if (ac < 5 || ac > 6)
	{
		fprintf(stderr, "%s\n", ERR_MSG);
		return (1);
	}
	if (check_args(++av))
	{
		return (1);
	}
	if (data_init(&data, av, ac))
	{
		if (pthread_init(&data))
			return (data.exit_status);
	}

	return (data.exit_status);
}
