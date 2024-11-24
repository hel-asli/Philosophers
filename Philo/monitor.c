/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:23:28 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 02:28:45 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	death_msg(t_philo *philo, size_t c)
{
	pthread_mutex_lock(&philo->data->msg_lock);
	printf("%zu %d is died\n", c - philo->data->start_time, philo->philo_id);
	pthread_mutex_unlock(&philo->data->msg_lock);
}

int	death_check(t_philo *philo)
{
	int		end;
	size_t	delay;
	size_t	c;

	c = get_current_time(MSECONDS);
	pthread_mutex_lock(&philo->data->last_meal_lock);
	delay = c - philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->last_meal_lock);
	if (delay > philo->data->time_die)
	{
		end = end_mutex_getter(philo->data);
		if (!end)
		{
			end_mutex_setter(philo->data, 1);
			death_msg(philo, c);
		}
		return (1);
	}
	return (0);
}

void	nb_meals_check(t_philo *philo)
{
	int	is_full;

	if (philo->data->nb_must_eat > 0)
	{
		is_full = full_mutex_getter(philo);
		if (!is_full)
		{
			pthread_mutex_lock(&philo->data->nb_meals_lock);
			if (philo->nb_meals >= philo->data->nb_must_eat)
				full_mutex_setter(philo, 1);
			pthread_mutex_unlock(&philo->data->nb_meals_lock);
		}
	}
}

int	all_full(t_philo *philo)
{
	size_t	i;

	i = 0;
	while (i < philo->data->nb_philos)
	{
		if (!philo[i].is_full)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_job(void *arg)
{
	t_philo	*philo;
	int		is_full;
	size_t	i;

	philo = arg;
	while (!all_full(philo))
	{
		i = 0;
		while (i < philo->data->nb_philos)
		{
			is_full = full_mutex_getter(&philo[i]);
			if (is_full)
			{
				i++;
				continue ;
			}
			if (death_check(&philo[i]))
				return (NULL);
			nb_meals_check(&philo[i]);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
