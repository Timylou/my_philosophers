/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:23:59 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/10 17:23:59 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdead(t_philosopher *philosopher, t_philo *philo)
{
	int	death_time;

	death_time = philosopher->last_meal + philo->time_to_die;
	return (ft_get_timestamp(philo->start_time) > death_time);
}

void	ft_print_thread(char *msg, t_philosopher *philosopher, t_philo *philo)
{
	int	id;
	int	is_dead;

	id = philosopher->id + 1;
	is_dead = ft_dead_access(philo);
	if (!is_dead)
	{
		pthread_mutex_lock(philo->print_access);
		printf("%ld\t%d %s\n", ft_get_timestamp(philo->start_time), id, msg);
		pthread_mutex_unlock(philo->print_access);
	}
}

static void	ft_wait_end(t_philo *philo)
{
	int	i;
	int	is_dead;
	int	ended;

	while (philo)
	{
		ended = 0;
		i = -1;
		pthread_mutex_lock(philo->dead_access);
		is_dead = philo->is_dead;
		pthread_mutex_unlock(philo->dead_access);
		while (++i < philo->nb_philo)
		{
			if (is_dead)
				break ;
			pthread_mutex_lock(philo->end_access);
			if (philo->philosophers[i]->end)
				ended++;
			pthread_mutex_unlock(philo->end_access);
		}
		if (ended == philo->nb_philo || is_dead)
			break ;
		usleep(300);
	}
}

t_philo	*ft_wait_threads(t_philo *philo)
{
	int				i;

	ft_wait_end(philo);
	i = -1;
	while (++i < philo->nb_philo)
	{
		if (pthread_join(philo->philosophers[i]->thread, NULL))
			return (ft_error(philo, "Error waiting threads\n"));
	}
	return (philo);
}

t_philo	*ft_launch(t_philo *philo)
{
	t_data			*data;
	t_philosopher	**philosophers;
	int				i;

	philo->start_time = ft_get_time() + (25 * philo->nb_philo);
	philosophers = philo->philosophers;
	i = 0;
	while (i < philo->nb_philo)
	{
		philosophers[i]->end = 0;
		data = malloc(sizeof(t_data));
		if (!data)
			return (ft_error(philo, "Data malloc error\n"));
		data->philo = philo;
		data->philosopher = philosophers[i];
		data->id = i;
		if (pthread_create(&philosophers[i]->thread, NULL, &ft_routine, data))
			return (ft_error(philo, "Thread creation error\n"));
		usleep(200);
		i++;
	}
	return (philo);
}
