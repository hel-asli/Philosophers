/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 07:05:54 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/21 20:56:59 by hel-asli         ###   ########.fr       */
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

void precise_usleep(size_t useconds)
{
    size_t start = get_current_time(MSECONDS);
    while ((get_current_time(MSECONDS) - start) < useconds)
	{
        usleep(500);
	}
}

void eat_phase(t_philo *philo)
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
	
	pthread_mutex_lock(&philo->data->last_meal_lock);
	philo->last_meal_time = get_current_time(MSECONDS);
	pthread_mutex_unlock(&philo->data->last_meal_lock);

    safe_print_msg(philo, EATING);
    precise_usleep(philo->data->time_eat);

    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void sleep_phase(t_philo *philo)
{
    safe_print_msg(philo, SLEPING);
    precise_usleep(philo->data->time_sleep);
}


void edge_case(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
    safe_print_msg(philo, FORK);

	pthread_mutex_lock(&philo->data->end_lock);	
	philo->data->end = 1;
	pthread_mutex_unlock(&philo->data->end_lock);	

	pthread_mutex_unlock(philo->left_fork);
}

int is_full(t_philo *philo)
{
	int n;

	pthread_mutex_lock(&philo->data->is_full_lock);
	n = philo->is_full;
	pthread_mutex_unlock(&philo->data->is_full_lock);


	return (n);
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!is_finish(philo->data) && !is_full(philo))
    {
		if (philo->data->nb_philos == 1)
			edge_case(philo);
		else
        	eat_phase(philo);
		if (is_finish(philo->data))
			break ;

		pthread_mutex_lock(&philo->data->nb_meals_lock);
		philo->nb_meals++;
		pthread_mutex_unlock(&philo->data->nb_meals_lock);

        sleep_phase(philo);
		if (is_finish(philo->data))
			break ;
        safe_print_msg(philo, THINKING);
    }
	if (philo->data->nb_philos == 1 && is_finish(philo->data))
		printf("%zu %d is died\n", get_current_time(MSECONDS) - philo->last_meal_time, philo->philo_id);
    return (NULL);
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
	data->start_time = get_current_time(MSECONDS);
	pthread_mutex_init(&data->msg_lock, NULL);
	pthread_mutex_init(&data->end_lock, NULL);
	pthread_mutex_init(&data->last_meal_lock, NULL);
    pthread_mutex_init(&data->nb_meals_lock, NULL);
	pthread_mutex_init(&data->is_full_lock, NULL);
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

int all_full(t_philo *philo)
{
	size_t i = 0;

	while (i < philo->data->nb_philos)
	{
		if (!philo[i].is_full)
			return (0);
		i++;
	}
	return (1);	
}

void *monitor_job(void *arg)
{
	size_t last_meal;
	size_t c;
	t_philo *philo;
	size_t i;

	philo = arg;
	while (philo->data->nb_philos > 1)
	{
		i = 0;

		if (all_full(philo))
		{
			break;
		}
		while (i < philo->data->nb_philos)
		{
			c = get_current_time(MSECONDS);
			// if (philo->data->end)
			// {
			// 	pthread_mutex_unlock(&philo->data->end_lock);
			// 	return (NULL);
			// }
			pthread_mutex_lock(&philo->data->last_meal_lock);
			last_meal = philo[i].last_meal_time;
			if (c - last_meal > philo->data->time_die)
			{
				pthread_mutex_lock(&philo->data->end_lock);
				if (!philo->data->end)
				{
					philo->data->end = 1;
					pthread_mutex_lock(&philo->data->msg_lock);
            		printf("%zu %d is died", c - philo->data->start_time, philo->philo_id);
					pthread_mutex_unlock(&philo->data->msg_lock);
				}
				pthread_mutex_unlock(&philo->data->end_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->data->last_meal_lock);

			if (philo->data->nb_must_eat > 0)
			{
			pthread_mutex_lock(&philo->data->is_full_lock);
			if (!philo[i].is_full)
			{
				pthread_mutex_lock(&philo->data->nb_meals_lock);
				if (philo[i].nb_meals >= philo->data->nb_must_eat)
				{
					philo[i].is_full = 1;
				}
				pthread_mutex_unlock(&philo->data->nb_meals_lock);
			}
			pthread_mutex_unlock(&philo->data->is_full_lock);
			}
			i++;
		}
		usleep(300);
	}
	return (NULL);
}

int philo_create(t_data *data)
{
	t_philo *philo;
	pthread_attr_t monitor_attr;
	pthread_attr_init(&monitor_attr);
	pthread_attr_setdetachstate(&monitor_attr, PTHREAD_CREATE_DETACHED);
	size_t i;

	i = 0;
	philo = malloc(sizeof(t_philo) * data->nb_philos);
	if (!philo)
	{
		destory_mutex(data, data->nb_philos);
		free(data->forks);
		return (1);
	}
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

    i = 0;
	pthread_create(&data->monitor, &monitor_attr, monitor_job, philo);
	while (i < data->nb_philos)
	{
		if (pthread_create(&philo[i].tid, NULL, philo_routine, &philo[i]))
			return (1);
		i++;
	}
	for (size_t i = 0; i < data->nb_philos; i++)
	{
		if (pthread_join(philo[i].tid, NULL))
		{
			destory_mutex(data, data->nb_philos);
			free(data->forks);
			free(philo);
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
    
    destory_mutex(data, data->nb_philos);
    free(data->forks);
	return (0);
}

size_t get_current_time(int flag)
{
	struct timeval timestamp;
	size_t ret;

	ret = 0;
	gettimeofday(&timestamp, NULL);
	if (flag == MSECONDS)
		ret = (timestamp.tv_sec * 1e3) + (timestamp.tv_usec / 1000);
	else if (flag == USECONDS)
		ret = (timestamp.tv_sec * 1e6) + (timestamp.tv_usec);

	return (ret);
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
