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

void	ft_print_thread(char *msg, int id, t_philo *philo)
{
	if (!philo->is_dead)
		printf("%ld\t%d %s\n", ft_get_timestamp(philo->start_time), id, msg);
}

void	*ft_end_thread(int is_dead, t_philosopher *philosopher, t_philo *philo)
{
	if (is_dead)
	{
		ft_print_thread("died", philosopher->id + 1, philo);
		philo->is_dead = 1;
	}
	philosopher->end = 1;
	return (NULL);
}

t_philo	*ft_wait_threads(t_philo *philo)
{
	int				i;
	int				ended;

	while (philo)
	{
		ended = 0;
		i = -1;
		while (++i < philo->nb_philo)
		{
			if (philo->is_dead)
				break ;
			if (philo->philosophers[i]->end)
				ended++;
		}
		if (ended == philo->nb_philo || philo->is_dead)
			break ;
	}
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

	philo->start_time = ft_get_time() + (250 * philo->nb_philo);
	philosophers = philo->philosophers;
	i = 0;
	while (i < philo->nb_philo)
	{
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
