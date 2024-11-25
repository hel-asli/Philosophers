/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 02:55:20 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/25 21:33:18 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	check_str(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
	{
		ft_putstr_fd(NOT_VALID_ERR, STDERR_FILENO);
		return (1);
	}
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			ft_putstr_fd(NOT_VALID_ERR, STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_digits(char *str)
{
	int	i;
	int	nb_digits;

	i = 0;
	nb_digits = 0;
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] == '0')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		nb_digits++;
		i++;
	}
	if (nb_digits >= 12)
	{
		ft_putstr_fd(BIG_NBR_ERR, STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	check_args(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (check_str(av[i]) || check_digits(av[i]))
			return (1);
		i++;
	}
	return (0);
}
