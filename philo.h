/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 06:17:09 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/16 05:26:25 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

typedef struct s_operations
{
	char *str;
	struct s_opernation *next;
} t_operations ;

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

#endif
