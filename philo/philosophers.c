/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:18:48 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/09 16:18:48 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philo	*philo;

	philo = ft_parse(argc, argv);
	if (!philo)
		return (EXIT_FAILURE);
	if (!ft_launch(philo))
		return (EXIT_FAILURE);
	if (!ft_wait_threads(philo))
		return (EXIT_FAILURE);
	ft_free_philo(philo);
}
