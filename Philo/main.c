/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/23 01:48:40 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
t_philo *philo_init(t_data *data)
{
	t_philo *philo;
	size_t	i;

	philo = malloc(sizeof(t_philo) * data->nb_philos);
	if (!philo)
	{
		destory_mutex(data, data->nb_philos);
		return (NULL);
	}
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

void join_philos(t_data *data, t_philo *philo)
{
	size_t	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(philo[i].tid, NULL))
			return ;
		i++;
	}
}
void philo_create(t_data *data)
{
	t_philo *philo;
	size_t i;
    i = 0;

	philo = philo_init(data);
	if (!philo)
		return ;
	while (i < data->nb_philos)
	{
		if (pthread_create(&philo[i].tid, NULL, philo_routine, &philo[i]))
			return ;
		i++;
	}
	monitor_create(data, philo);
	join_philos(data, philo);
	free(philo);
}


int	pthread_init(t_data *data)
{
	if (fork_mutex_init(data))
		return (1);
	philo_create(data);
    
    destory_mutex(data, data->nb_philos);
    free(data->forks);
	return (0);
}

int	main(int ac, char **av)
{
	t_data data;

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
	}

	return (data.exit_status);
}
