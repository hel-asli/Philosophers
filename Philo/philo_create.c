/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 00:01:59 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/25 21:46:50 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destory_mutex(t_data *data, size_t i)
{
	size_t	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&data->forks[j]);
		j++;
	}
}

int	fork_mutex_init(t_data *data)
{
	size_t	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

t_philo	*philo_init(t_data *data)
{
	size_t	i;
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * data->nb_philos);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < data->nb_philos)
	{
		philo[i].data = data;
		philo[i].nb_meals = 0;
		philo[i].is_full = 0;
		philo[i].left_fork = &data->forks[i];
		philo[i].right_fork = &data->forks[(i + 1) % (data->nb_philos)];
		philo[i].philo_id = i + 1;
		philo[i].last_meal_time = data->start_time;
		i++;
	}
	return (philo);
}

void	join_threads(t_philo *philo, t_data *data)
{
	size_t	i;

	i = 0;
	while (i < philo->data->nb_philos)
	{
		if (pthread_join(philo[i].tid, NULL))
			return ;
		i++;
	}
	pthread_join(data->monitor, NULL);
}

void	philo_create(t_data *data)
{
	t_philo	*philo;
	size_t	i;

	philo = philo_init(data);
	if (!philo)
		return ;
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&philo[i].tid, NULL, philo_routine, &philo[i]))
			return ;
		i++;
	}
	pthread_create(&data->monitor, NULL, monitor_job, philo);
	join_threads(philo, data);
	free(philo);
}
