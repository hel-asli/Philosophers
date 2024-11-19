/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/19 21:41:14 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	printf("%zu ms %d has taken a fork\n", get_current_time(), philo->philo_id);
	pthread_mutex_lock(philo->right_fork);
	printf("%zu ms %d has taken a fork\n", get_current_time(), philo->philo_id);

	pthread_mutex_lock(philo->data->msg_lock);
	printf("%zu ms %d is eating \n", get_current_time(), philo->philo_id);
	usleep(philo->data->time_eat);
	pthread_mutex_unlock(philo->data->msg_lock);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}


void sleepp(t_philo *philo)
{
	pthread_mutex_lock(philo->data->msg_lock);
	printf("%zu ms %d is sleeping \n", get_current_time(), philo->philo_id);
	usleep(philo->data->time_sleep);
	pthread_mutex_unlock(philo->data->msg_lock);
}

void think(t_philo *philo)
{
	pthread_mutex_lock(philo->data->msg_lock);
	printf("%zu ms %d is think \n", get_current_time(), philo->philo_id);
	usleep(300);
	pthread_mutex_unlock(philo->data->msg_lock);
}

void *philo_routine(void *arg)
{
	t_philo *philo;

	philo = arg;
	while (!philo->death)
	{
		eat(philo);
		sleepp(philo);
		think(philo);
	}
	return (arg);
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
	data->msg_lock = malloc(sizeof(pthread_mutex_t));
	if (!data->msg_lock)
	{
		destory_mutex(data, data->nb_philos);
		free(data->forks);
		free(data->philo);
		return (1);
	}
	pthread_mutex_init(data->msg_lock, NULL);
	while (i < data->nb_philos)
	{
		data->philo[i].data = data;
		data->philo[i].philo_id = i + 1;
		data->philo[i].left_fork = &data->forks[i];
		data->philo[i].right_fork = &data->forks[(i + 1) % (data->nb_philos)];
		if (pthread_create(&data->philo[i].tid, NULL, philo_routine, data->philo))
			return (1);
		i++;
	}
	printf("%zu\n", i);
	pause();
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

size_t get_current_time(void)
{
	struct timeval timestamp;

	gettimeofday(&timestamp, NULL);

	return ((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000));
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
			return (1);
	}

	return (data.exit_status);
}
