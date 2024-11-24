/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 06:07:11 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_usleep(size_t useconds)
{
	size_t	start;

	start = get_current_time(MSECONDS);
	while ((get_current_time(MSECONDS) - start) < useconds)
	{
		usleep(500);
	}
}

void	destroy_data_mutex(t_data *data)
{
	pthread_mutex_destroy(&data->is_full_lock);
	pthread_mutex_destroy(&data->end_lock);
	pthread_mutex_destroy(&data->last_meal_lock);
	pthread_mutex_destroy(&data->msg_lock);
}

int	pthread_init(t_data *data)
{
	fork_mutex_init(data);
	philo_create(data);
	destory_mutex(data, data->nb_philos);
	free(data->forks);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
	{
		ft_putstr_fd(ERR_MSG, STDERR_FILENO);
		return (1);
	}
	if (check_args(++av))
		return (1);
	if (data_init(&data, av, ac))
	{
		if (pthread_init(&data))
			return (1);
		destroy_data_mutex(&data);
	}
	return (data.exit_status);
}
