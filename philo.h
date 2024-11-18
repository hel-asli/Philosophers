/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 06:17:09 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/18 16:14:22 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define ERR_MSG "Error: Invalid arguments.\n\
Usage: ./philosophers number_of_philosophers\
 time_to_die time_to_eat\
 time_to_sleep\
 [number_of_times_each_philosopher_must_eat]\n"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_philo
{
	pthread_t tid;
	int		philo_id;
} t_philo;

typedef struct s_data
{
	size_t	nb_philos;
	size_t	time_die;
	size_t	time_eat;
	size_t	time_sleep;
	size_t	nb_must_eat;
	int		exit_status;
	t_philo *philo;
	pthread_mutex_t *forks;
} t_data;

#endif
