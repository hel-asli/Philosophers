/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:28:22 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/23 01:04:56 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_atoi(char *str)
{
	int		i;
	size_t	r;

	r = 0;
	i = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - 48);
		i++;
	}
	return (r);
}

void ft_usleep(size_t useconds)
{
    size_t start = get_current_time(MSECONDS);

    while ((get_current_time(MSECONDS) - start) < useconds)
	{
        usleep(500);
	}
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

void  ft_error(t_data *data, char *str)
{
	data->exit_status = 1;	
	ft_putstr_fd(str, STDERR_FILENO);
}

size_t ft_strlen(char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	size;

	if (s)
	{
		size = ft_strlen(s);
		write(fd, s, size);
	}
}