#include "philo.h"

void safe_print_msg(t_philo *philo, t_msg state)
{
    size_t current_time;
    size_t elapsed_time;

    current_time = get_current_time(MSECONDS);
    elapsed_time = current_time - philo->data->start_time;
    pthread_mutex_lock(&philo->data->msg_lock);
    if (!is_finish(philo->data))
    {
        if (state == FORK)
            printf("%zu %d has taken a fork\n", 
                elapsed_time, philo->philo_id);
        else if (state == EATING)
            printf("%zu %d is eating\n", elapsed_time, philo->philo_id);
        else if (state == SLEPING)
            printf("%zu %d is sleeping\n", elapsed_time, philo->philo_id);
        else if (state == THINKING)
            printf("%zu %d is thinking\n", elapsed_time, philo->philo_id);
    }
    pthread_mutex_unlock(&philo->data->msg_lock);
}

