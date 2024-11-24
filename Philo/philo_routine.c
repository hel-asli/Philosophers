/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 02:55:57 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 03:03:12 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		safe_print_msg(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		safe_print_msg(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		safe_print_msg(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		safe_print_msg(philo, FORK);
	}
}

void	eat_phase(t_philo *philo)
{
	lock_forks(philo);
	pthread_mutex_lock(&philo->data->last_meal_lock);
	philo->last_meal_time = get_current_time(MSECONDS);
	pthread_mutex_unlock(&philo->data->last_meal_lock);
	safe_print_msg(philo, EATING);
	precise_usleep(philo->data->time_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_phase(t_philo *philo)
{
	safe_print_msg(philo, SLEPING);
	precise_usleep(philo->data->time_sleep);
}

void	edge_case(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_print_msg(philo, FORK);
	precise_usleep(philo->data->time_die);
	pthread_mutex_unlock(philo->left_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 1)
		usleep(50);
	while (!end_mutex_getter(philo->data) && !full_mutex_getter(philo))
	{
		if (philo->data->nb_philos == 1)
			edge_case(philo);
		else
			eat_phase(philo);
		pthread_mutex_lock(&philo->data->nb_meals_lock);
		philo->nb_meals++;
		pthread_mutex_unlock(&philo->data->nb_meals_lock);
		if (end_mutex_getter(philo->data))
			break ;
		sleep_phase(philo);
		if (end_mutex_getter(philo->data))
			break ;
		safe_print_msg(philo, THINKING);
	}
	return (NULL);
}
