/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:25:08 by yel-mens          #+#    #+#             */
/*   Updated: 2025/10/07 10:25:10 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_lonely_philo(t_philosopher *philosopher, t_philo *philo)
{
	pthread_mutex_lock(philosopher->r_fork);
	ft_print_thread("has taken a fork", philosopher, philo);
	usleep(philo->time_to_die * 1000);
	pthread_mutex_unlock(philosopher->r_fork);
	ft_print_thread("died", philosopher, philo);
	pthread_mutex_lock(philo->dead_access);
	philo->is_dead = 1;
	pthread_mutex_unlock(philo->dead_access);
	pthread_mutex_lock(philo->end_access);
	philosopher->end = 1;
	pthread_mutex_unlock(philo->end_access);
	return (NULL);
}

int	ft_dead_access(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(philo->dead_access);
	is_dead = philo->is_dead;
	pthread_mutex_unlock(philo->dead_access);
	return (is_dead);
}

int	ft_lock_mutexs(t_philosopher *philosopher, t_philo *philo)
{
	if (philosopher->r_fork < philosopher->l_fork)
		pthread_mutex_lock(philosopher->r_fork);
	else
		pthread_mutex_lock(philosopher->l_fork);
	ft_print_thread("has taken a fork", philosopher, philo);
	if (ft_isdead(philosopher, philo))
	{
		if (philosopher->r_fork < philosopher->l_fork)
			pthread_mutex_unlock(philosopher->r_fork);
		else
			pthread_mutex_unlock(philosopher->l_fork);
		return (0);
	}
	if (philosopher->r_fork < philosopher->l_fork)
		pthread_mutex_lock(philosopher->l_fork);
	else
		pthread_mutex_lock(philosopher->r_fork);
	ft_print_thread("has taken a fork", philosopher, philo);
	if (ft_isdead(philosopher, philo))
	{
		pthread_mutex_unlock(philosopher->r_fork);
		pthread_mutex_unlock(philosopher->l_fork);
		return (0);
	}
	return (1);
}
