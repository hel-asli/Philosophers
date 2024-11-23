/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encapsulate_mutex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:19:05 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/23 23:39:16 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int end_mutex_getter(t_data *data)
{
	int end;

	pthread_mutex_lock(&data->end_lock);
	end = data->end;
	pthread_mutex_unlock(&data->end_lock);

	return (end);
}

void end_mutex_setter(t_data *data, int value)
{
	pthread_mutex_lock(&data->end_lock);
	data->end = value;
	pthread_mutex_unlock(&data->end_lock);
}

int	full_mutex_getter(t_philo *philo)
{
	int is_full;

	pthread_mutex_lock(&philo->data->is_full_lock);
	is_full = philo->is_full;
	pthread_mutex_unlock(&philo->data->is_full_lock);

	return (is_full);
}

void	full_mutex_setter(t_philo *philo, int value)
{
	pthread_mutex_lock(&philo->data->is_full_lock);
	philo->is_full = value;
	pthread_mutex_unlock(&philo->data->is_full_lock);

}