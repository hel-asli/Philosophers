/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 06:17:09 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/18 21:15:56 by hel-asli         ###   ########.fr       */
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
# include <string.h>

# define FAILURE 1
# define SUCESS 0

typedef struct s_philo
{
	pthread_t tid;
	int		philo_id;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
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

bool	is_space(char c);
int		count_words(char *str);
int		check_str(char *str);
int		check_digits(char *str);
int		check_args(char **av);
size_t	ft_atoi(char *str);

#endif
