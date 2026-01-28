/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:15:54 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/10 13:15:54 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_wait_start(time_t start_time, int id)
{
	while (ft_get_time() < start_time + (id % 2) * 20)
		;
}

time_t	ft_get_timestamp(time_t start_time)
{
	return (ft_get_time() - start_time);
}

time_t	ft_get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
