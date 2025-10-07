/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:19:11 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/09 16:19:11 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philosopher
{
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_t		thread;
	int				time_ate;
	time_t			last_meal;
	int				end;
	int				id;
}				t_philosopher;

typedef struct s_philo
{
	int				nb_philo;
	time_t			start_time;
	time_t			time_to_think;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				is_dead;
	int				end;
	int				goal_eat;
	t_philosopher	**philosophers;
}				t_philo;

typedef struct s_data
{
	t_philo			*philo;
	t_philosopher	*philosopher;
	int				id;
}				t_data;

/* * * * * *
*  utils *
* * * * * */

int		ft_atoi(char *str);
int		ft_strlen(char *str);

/* * * * * 
* free *
* * * * */

void	ft_free_philo(t_philo *philo);
void	*ft_error(t_philo *philo, char *error);
void	*ft_close_mutex(t_philo *philo, int index_mutex);

/* * * * * 
* parse *
* * * * * */

t_philo	*ft_parse(int argc, char **argv);

/* * * * * 
*  time *
* * * * * */

time_t	ft_get_timestamp(time_t start_time);
void	ft_wait_start(time_t start_time, int id);
time_t	ft_get_time(void);

/* * * * * *
* threads *
* * * * * * */

t_philo	*ft_wait_threads(t_philo *philo);
void	*ft_end_thread(int isdead, t_philosopher *philosopher, t_philo *philo);
void	ft_print_thread(char *msg, int id, t_philo *philo);
t_philo	*ft_launch(t_philo *philo);

/* * * * * * *
*  routine *
* * * * * * */

void	*ft_routine(void *data);
int		ft_isdead(t_philosopher *philosopher, t_philo *philo);

#endif