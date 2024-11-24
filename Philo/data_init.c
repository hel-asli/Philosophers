/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:06:22 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 02:36:42 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	data_init_arg(t_data *data, char **av, int ac)
{
	data->end = 0;
	data->start_time = get_current_time(MSECONDS);
	data->nb_must_eat = 0;
	data->nb_philos = ft_atoi(av[0]);
	if (!data->nb_philos || data->nb_philos > 200)
		return (ft_error(data, NBPHILOS_ERR_MSG), 1);
	data->time_die = ft_atoi(av[1]);
	data->time_eat = ft_atoi(av[2]);
	data->time_sleep = ft_atoi(av[3]);
	if (data->time_die < 60 || data->time_eat < 60 || data->time_sleep < 60)
		return (ft_error(data, TIME_ERR_MSG), 1);
	if (ac == 6)
	{
		data->nb_must_eat = ft_atoi(av[4]);
		if (!data->nb_must_eat)
			return (ft_error(data, NBPHILOS_ERR_MSG), 1);
	}
	return (0);
}

int	data_mutex_init(t_data *data)
{
	if (pthread_mutex_init(&data->msg_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->end_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->last_meal_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->nb_meals_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->is_full_lock, NULL))
		return (1);
	return (0);
}

int	data_init(t_data *data, char **av, int ac)
{
	if (data_init_arg(data, av, ac))
		return (1);
	if (data_mutex_init(data))
		return (1);
	return (1);
}
