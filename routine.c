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
	pthread_mutex_lock(philosopher->r_fork);
	ft_print_thread("has taken a fork", philosopher->id + 1, philo);
	if (ft_isdead(philosopher, philo))
	{
		pthread_mutex_unlock(philosopher->r_fork);
		return (0);
	}
	if (philo->nb_philo > 1)
		pthread_mutex_lock(philosopher->l_fork);
	ft_print_thread("has taken a fork", philosopher->id + 1, philo);
	if (ft_isdead(philosopher, philo))
	{
		pthread_mutex_unlock(philosopher->r_fork);
		if (philo->nb_philo > 1)
			pthread_mutex_unlock(philosopher->l_fork);
		return (0);
	}
	ft_print_thread("is eating", philosopher->id + 1, philo);
	usleep(philo->time_to_eat * 1000);
	philosopher->last_meal = ft_get_timestamp(philo->start_time);
	pthread_mutex_unlock(philosopher->r_fork);
	if (philo->nb_philo > 1)
		pthread_mutex_unlock(philosopher->l_fork);
	philosopher->time_ate++;
	return (1);
}

static int	ft_sleeping_routine(t_philosopher *philosopher, t_philo *philo)
{
	time_t	time;
	time_t	end_sleeping;

	ft_print_thread("is sleeping", philosopher->id + 1, philo);
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

static void	ft_thinking_routine(t_philosopher *philosopher, t_philo *philo)
{
	ft_print_thread("is thinking", philosopher->id + 1, philo);
	usleep(philo->time_to_think);
}

static int	ft_check_routine(t_philosopher *philosopher, t_philo *philo)
{
	if (philo->is_dead)
		return (0);
	if (ft_isdead(philosopher, philo))
	{
		ft_print_thread("died", philosopher->id + 1, philo);
		philo->is_dead = 1;
		return (0);
	}
	if (philo->goal_eat > 0)
		return (philosopher->time_ate < philo->goal_eat);
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
	philosopher->end = 0;
	ft_wait_start(philo->start_time, philosopher->id);
	while (ft_check_routine(philosopher, philo))
	{
		if (!ft_eat_routine(philosopher, philo))
			return (ft_end_thread(1, philosopher, philo));
		if (philo->is_dead)
			return (ft_end_thread(0, philosopher, philo));
		if (!ft_sleeping_routine(philosopher, philo))
			return (ft_end_thread(1, philosopher, philo));
		if (philo->is_dead)
			return (ft_end_thread(0, philosopher, philo));
		ft_thinking_routine(philosopher, philo);
	}
	return (ft_end_thread(0, philosopher, philo));
}
