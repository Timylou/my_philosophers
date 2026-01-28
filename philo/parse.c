/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:27:50 by yel-mens          #+#    #+#             */
/*   Updated: 2025/10/07 09:45:03 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philo	*ft_init_philo(void)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->nb_philo = -1;
	philo->start_time = -1;
	philo->time_to_die = -1;
	philo->time_to_eat = -1;
	philo->time_to_think = -1;
	philo->time_to_sleep = -1;
	philo->is_dead = 0;
	philo->goal_eat = -1;
	philo->philosophers = NULL;
	philo->dead_access = malloc(sizeof(pthread_mutex_t));
	if (!philo->dead_access || pthread_mutex_init(philo->dead_access, NULL))
		return (ft_error(philo, "Mutex cannot init\n"));
	philo->end_access = malloc(sizeof(pthread_mutex_t));
	if (!philo->end_access || pthread_mutex_init(philo->end_access, NULL))
		return (ft_error(philo, "Mutex cannot init\n"));
	philo->print_access = malloc(sizeof(pthread_mutex_t));
	if (!philo->print_access || pthread_mutex_init(philo->print_access, NULL))
		return (ft_error(philo, "Mutex cannot init\n"));
	return (philo);
}

static t_philo	*ft_init_argv(int argc, char **argv, t_philo *philo)
{
	int	eat_plus_sleep;

	philo->nb_philo = ft_atoi(argv[1]);
	if (philo->nb_philo < 0)
		return (ft_error(philo, "Arguments must have only digits\n"));
	philo->time_to_die = ft_atoi(argv[2]);
	if (philo->time_to_die < 0)
		return (ft_error(philo, "Arguments must have only digits\n"));
	philo->time_to_eat = ft_atoi(argv[3]);
	if (philo->time_to_eat < 0)
		return (ft_error(philo, "Arguments must have only digits\n"));
	philo->time_to_sleep = ft_atoi(argv[4]);
	if (philo->time_to_sleep < 0)
		return (ft_error(philo, "Arguments must have only digits\n"));
	philo->goal_eat = -1;
	if (argc == 6)
		philo->goal_eat = ft_atoi(argv[5]);
	if (philo->goal_eat < 0 && argc == 6)
		return (ft_error(philo, "Arguments must have only digits\n"));
	eat_plus_sleep = philo->time_to_eat + philo->time_to_sleep;
	philo->time_to_think = philo->time_to_die - eat_plus_sleep;
	if (philo->time_to_think < 0)
		philo->time_to_think = 0;
	return (philo);
}

static t_philo	*ft_init_philosophers(t_philo *philo)
{
	t_philosopher	**philosophers;
	int				i;

	philosophers = malloc(sizeof(t_philosopher *) * philo->nb_philo);
	philo->philosophers = philosophers;
	if (!philosophers)
		return (ft_error(philo, "Malloc error\n"));
	i = -1;
	while (++i < philo->nb_philo)
	{
		philosophers[i] = malloc(sizeof(t_philosopher));
		if (!philosophers[i])
		{
			while (i-- > 0)
				free(philosophers[i]);
			free(philosophers);
			philo->philosophers = NULL;
			return (ft_error(philo, "Malloc error\n"));
		}
		philosophers[i]->time_ate = 0;
		philosophers[i]->last_meal = 0;
		philosophers[i]->r_fork = NULL;
		philosophers[i]->l_fork = NULL;
	}
	return (philo);
}

static t_philo	*ft_init_fork(t_philo *philo)
{
	t_philosopher	**philosophers;
	int				i;

	if (philo->nb_philo < 1)
		return (ft_error(philo, "You must have at least 1 philosopher\n"));
	philosophers = philo->philosophers;
	philosophers[0]->r_fork = malloc(sizeof(pthread_mutex_t));
	if (!philosophers[0]->r_fork)
		return (ft_close_mutex(philo, 0));
	if (pthread_mutex_init(philosophers[0]->r_fork, NULL))
		return (ft_close_mutex(philo, 0));
	i = 0;
	while (++i < philo->nb_philo)
	{
		philosophers[i]->l_fork = philosophers[i - 1]->r_fork;
		philosophers[i]->r_fork = malloc(sizeof(pthread_mutex_t));
		if (!philosophers[i]->r_fork)
			return (ft_close_mutex(philo, i));
		if (pthread_mutex_init(philosophers[i]->r_fork, NULL))
			return (ft_close_mutex(philo, i));
	}
	if (philo->nb_philo > 1)
		philosophers[0]->l_fork = philosophers[philo->nb_philo - 1]->r_fork;
	return (philo);
}

t_philo	*ft_parse(int argc, char **argv)
{
	t_philo	*philo;

	if (argc != 5 && argc != 6)
	{
		write(STDERR_FILENO, "./philo nb_philo time_to_die time_to_eat ", 41);
		write(STDERR_FILENO, "time_to_sleep (nb_of_time_philo_must_eat)\n", 42);
		return (NULL);
	}
	philo = ft_init_philo();
	if (!philo)
		return (NULL);
	philo->philosophers = NULL;
	if (!ft_init_argv(argc, argv, philo))
		return (NULL);
	if (!ft_init_philosophers(philo))
		return (NULL);
	if (!ft_init_fork(philo))
		return (NULL);
	return (philo);
}
