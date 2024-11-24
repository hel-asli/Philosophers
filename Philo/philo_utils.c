/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:14:48 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 02:41:09 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_print_msg(t_philo *philo, t_msg state)
{
	size_t	current_time;
	size_t	elapsed_time;
	int		end;

	current_time = get_current_time(MSECONDS);
	elapsed_time = current_time - philo->data->start_time;
	end = end_mutex_getter(philo->data);
	if (!end)
	{
		pthread_mutex_lock(&philo->data->msg_lock);
		if (state == FORK)
			printf("%zu %d has taken a fork\n",
				elapsed_time, philo->philo_id);
		else if (state == EATING)
			printf("%zu %d is eating\n", elapsed_time, philo->philo_id);
		else if (state == SLEPING)
			printf("%zu %d is sleeping\n", elapsed_time, philo->philo_id);
		else if (state == THINKING)
			printf("%zu %d is thinking\n", elapsed_time, philo->philo_id);
		pthread_mutex_unlock(&philo->data->msg_lock);
	}
}
