/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-mens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 16:46:12 by yel-mens          #+#    #+#             */
/*   Updated: 2025/07/09 16:46:12 by yel-mens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_atoi(char *str)
{
	int	num;

	num = 0;
	while (*str)
	{
		if (!('0' <= *str && *str <= '9'))
			return (-1);
		num = num * 10 + (*str - '0');
		str++;
	}
	return (num);
}
