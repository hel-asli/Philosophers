/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 06:17:09 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 00:21:41 by hel-asli         ###   ########.fr       */
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
# include <sys/time.h>

# define FAILURE 1
# define SUCESS 0

# define NBEAT_ERR_MSG "number_of_times_each_philosopher_must_eat not valid\n"
# define NBPHILOS_ERR_MSG "number_of_philosophers not valid\n"
# define TIME_ERR_MSG "time_to_die, time_to_eat, time_to_sleep must be greather than 60\n"

# define MSECONDS 1
# define USECONDS 2

typedef struct s_data t_data;


typedef enum e_msg
{
	FORK,
	EATING,
	SLEPING,
	THINKING,
} t_msg;


typedef struct s_philo
{
	pthread_t		tid;
	int				philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	size_t			last_meal_time;
	size_t			nb_meals;
	int				is_full; // is lock setter implement
	t_data			*data;
} t_philo;


typedef struct s_data
{
	pthread_t		monitor;
	size_t			nb_philos;
	size_t			time_die;
	size_t			time_eat;
	size_t			time_sleep;
	size_t			nb_must_eat;
	int				exit_status;
	pthread_mutex_t last_meal_lock;
	pthread_mutex_t nb_meals_lock;
	size_t			start_time;
	int				end;
	pthread_mutex_t is_full_lock;
	pthread_mutex_t	end_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	msg_lock;
} t_data;

bool	is_space(char c);
int		count_words(char *str);
int		check_str(char *str);
int		check_digits(char *str);
int		check_args(char **av);
size_t	ft_atoi(char *str);
size_t	get_current_time(int flag);
void	safe_print_msg(t_philo *philo, t_msg state);
int		data_init(t_data *data, char **av, int ac);
int		pthread_init(t_data *data);
void		fork_mutex_init(t_data *data);
void	destory_mutex(t_data *data, size_t i);
void	ft_putstr_fd(char *s, int fd);
void 	ft_error(t_data *data, char *str);
int		end_mutex_getter(t_data *data);
void	end_mutex_setter(t_data *data, int value);
int		full_mutex_getter(t_philo *philo);
void	full_mutex_setter(t_philo *philo, int value);
void	*monitor_job(void *arg);
void	*philo_routine(void *arg);

#endif
