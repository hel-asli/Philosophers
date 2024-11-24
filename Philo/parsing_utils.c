/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 02:55:20 by hel-asli          #+#    #+#             */
/*   Updated: 2024/11/24 02:55:24 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	count_words(char *str)
{
	int	i;
	int	w;

	i = 0;
	w = 0;
	while (is_space(str[i]))
		i++;
	while (str[i])
	{
		if (!is_space(str[i]) && (!str[i + 1] || is_space(str[i + 1])))
			w++;
		i++;
	}
	return (w);
}

int	check_str(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
	{
		fprintf(stderr, "Error: Arg not valid\n");
		return (1);
	}
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			fprintf(stderr, "Error: Arg not valid\n");
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
		fprintf(stderr, "number to big\n");
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
