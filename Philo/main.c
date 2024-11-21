/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/21 01:38:04 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_finish(t_data *data)
{
	int n;

	pthread_mutex_lock(&data->end_lock);
	n = data->end;
	pthread_mutex_unlock(&data->end_lock);

	return (n);
}

void print_msg(t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->data->msg_lock);
	if (!is_finish(philo->data))
		printf("%zu ms %d %s\n", get_current_time() - philo->data->start_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->msg_lock);
}


void eat_phase(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, FORK_MSG);
		pthread_mutex_lock(philo->right_fork);
		print_msg(philo, FORK_MSG);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_msg(philo, FORK_MSG);
		pthread_mutex_lock(philo->left_fork);
		print_msg(philo, FORK_MSG);
	}
	pthread_mutex_lock(&philo->data->last_meal_lock);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->data->last_meal_lock);

	print_msg(philo, EAT_MSG);
	usleep(philo->data->time_eat * 1000);

	pthread_mutex_lock(&philo->nb_meals_lock);
	philo->nb_meals += 1;
	pthread_mutex_unlock(&philo->nb_meals_lock);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}


void sleep_phase(t_philo *philo)
{
	print_msg(philo, SLEEP_MSG);
	usleep(philo->data->time_sleep * 1000);
}


void *philo_routine(void *arg)
{
	t_philo *philo;

	philo = arg;
	while (!is_finish(philo->data))
	{
		eat_phase(philo);
		sleep_phase(philo);
		print_msg(philo, THINK_MSG);
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
	data->end = 0;	
	data->start_time = get_current_time();
	pthread_mutex_init(&data->msg_lock, NULL);
	pthread_mutex_init(&data->end_lock, NULL);
	pthread_mutex_init(&data->last_meal_lock, NULL);
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

void *monitor_job(void *arg)
{
	size_t timestamp;
	size_t delay;
	t_data *data;
	size_t i;

	data = arg;
	while (!is_finish(data))
	{
		i = 0;
		while (i < data->nb_philos)
		{
			timestamp = get_current_time();
			pthread_mutex_lock(&data->last_meal_lock);
			delay = timestamp - data->philo[i].last_meal_time;
			pthread_mutex_unlock(&data->last_meal_lock);
			if (delay > data->time_die)
			{
				pthread_mutex_lock(&data->end_lock);
				data->end = 1;
				pthread_mutex_lock(&data->msg_lock);
				printf("%zu ms %d is died\n", get_current_time() - data->start_time, data->philo[i].philo_id);
				pthread_mutex_unlock(&data->msg_lock);
				pthread_mutex_unlock(&data->end_lock);
				return (NULL);
			}
			i++;
		}
		usleep(300);
	}
	return (NULL);
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
		pthread_mutex_init(&data->philo[i].nb_meals_lock, NULL);
		data->philo[i].data = data;
		data->philo[i].nb_meals = 0;
		data->philo[i].philo_id = i + 1;
		data->philo[i].left_fork = &data->forks[i];
		data->philo[i].right_fork = &data->forks[(i + 1) % (data->nb_philos)];
		data->philo[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philo[i].tid, NULL, philo_routine, &data->philo[i]))
			return (1);
		i++;
	}
	pthread_create(&data->monitor, NULL, monitor_job, data);
	pthread_detach(data->monitor);
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

	if (ac < 5 || ac > 6)
	{
		fprintf(stderr, "%s\n", ERR_MSG);
		return (1);
	}
	if (check_args(++av))
		return (1);
	if (data_init(&data, av, ac))
	{
		if (pthread_init(&data))
			return (1);
	}

	return (data.exit_status);
}
