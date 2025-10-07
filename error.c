/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:00:46 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/09 17:00:46 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	ft_free_philosophers(t_philo *philo)
{
	t_philosopher	**philosophers;
	int				i;

	philosophers = philo->philosophers;
	i = 0;
	while (i < philo->nb_philo)
	{
		pthread_mutex_destroy(philosophers[i]->r_fork);
		free(philosophers[i]->r_fork);
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
	philo->philosophers = NULL;
}

void	ft_free_philo(t_philo *philo)
{
	if (!philo)
		return ;
	if (philo->philosophers)
		ft_free_philosophers(philo);
	free(philo);
}

void	*ft_error(t_philo *philo, char *error)
{
	write(STDERR_FILENO, error, ft_strlen(error));
	ft_free_philo(philo);
	return (NULL);
}

void	*ft_close_mutex(t_philo *philo, int index_mutex)
{
	t_philosopher	**philosophers;
	int				i;

	philosophers = philo->philosophers;
	i = 0;
	while (i < philo->nb_philo)
	{
		if (i < index_mutex && philosophers[i]->r_fork)
			pthread_mutex_destroy(philosophers[i]->r_fork);
		if (philosophers[i]->r_fork)
			free(philosophers[i]->r_fork);
		free(philosophers[i]);
		i++;
	}
	free(philosophers);
	philo->philosophers = NULL;
	return (ft_error(philo, "Cannot init mutex\n"));
}
