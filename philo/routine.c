/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:29:51 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/10 17:29:51 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_eat_routine(t_philosopher *philosopher, t_philo *philo)
{
	time_t	time;
	time_t	end_eating;

	if (!ft_lock_mutexs(philosopher, philo))
		return (0);
	time = ft_get_timestamp(philo->start_time);
	ft_print_thread("is eating", philosopher, philo);
	end_eating = time + philo->time_to_eat;
	while (time < end_eating)
	{
		if (time > philosopher->last_meal + philo->time_to_die)
		{
			pthread_mutex_unlock(philosopher->r_fork);
			pthread_mutex_unlock(philosopher->l_fork);
			return (0);
		}
		if (time + 5 < end_eating)
			usleep(5);
		time = ft_get_timestamp(philo->start_time);
	}
	philosopher->last_meal = ft_get_timestamp(philo->start_time);
	pthread_mutex_unlock(philosopher->r_fork);
	pthread_mutex_unlock(philosopher->l_fork);
	philosopher->time_ate++;
	return (1);
}

static int	ft_sleep_routine(t_philosopher *philosopher, t_philo *philo)
{
	time_t	time;
	time_t	end_sleeping;

	ft_print_thread("is sleeping", philosopher, philo);
	time = ft_get_timestamp(philo->start_time);
	end_sleeping = time + philo->time_to_sleep;
	while (time < end_sleeping)
	{
		if (time > philosopher->last_meal + philo->time_to_die)
			return (0);
		if (time + 5 < end_sleeping)
			usleep(5);
		time = ft_get_timestamp(philo->start_time);
	}
	return (1);
}

static void	ft_think_routine(t_philosopher *philosopher, t_philo *philo)
{
	ft_print_thread("is thinking", philosopher, philo);
	usleep(philo->time_to_think);
}

static int	ft_check_routine(t_philosopher *philosopher, t_philo *philo)
{
	int	is_dead;
	int	id;

	is_dead = ft_dead_access(philo);
	if (is_dead)
		return (0);
	if (ft_isdead(philosopher, philo))
	{
		pthread_mutex_lock(philo->dead_access);
		philo->is_dead += 1;
		pthread_mutex_unlock(philo->dead_access);
		usleep(10);
		id = philosopher->id + 1;
		pthread_mutex_lock(philo->dead_access);
		if (philo->is_dead == 1)
		{
			pthread_mutex_lock(philo->print_access);
			printf("%ld\t%d died\n", ft_get_timestamp(philo->start_time), id);
			pthread_mutex_unlock(philo->print_access);
		}
		pthread_mutex_unlock(philo->dead_access);
		return (0);
	}
	return (1);
}

void	*ft_routine(void *data)
{
	t_philo			*philo;
	t_philosopher	*philosopher;

	philo = ((t_data *)data)->philo;
	philosopher = ((t_data *)data)->philosopher;
	philosopher->id = ((t_data *)data)->id;
	free(data);
	ft_wait_start(philo->start_time, philosopher->id);
	if (philo->nb_philo == 1)
		return (ft_lonely_philo(philosopher, philo));
	while (ft_check_routine(philosopher, philo))
	{
		if (!ft_eat_routine(philosopher, philo))
			continue ;
		if (philo->goal_eat > 0 && philosopher->time_ate >= philo->goal_eat)
			break ;
		if (!ft_sleep_routine(philosopher, philo))
			continue ;
		ft_think_routine(philosopher, philo);
	}
	pthread_mutex_lock(philo->end_access);
	philosopher->end = 1;
	pthread_mutex_unlock(philo->end_access);
	return (NULL);
}
