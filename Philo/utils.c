#include "philo.h"

size_t ft_atoi(char *str)
{
	int i = 0;
	size_t r;

	r = 0;
	while (str[i] && is_space(str[i]))
		i++;	
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - 48) ;
		i++;
	}
	return (r);
}
