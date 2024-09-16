/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:00:09 by jmouette          #+#    #+#             */
/*   Updated: 2024/05/22 10:39:20 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar_len(char c)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	return (1);
}

int	ft_string(char *str)
{
	int	len;

	len = 0;
	if (!str)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		len = 6;
		return (len);
	}
	while (*str)
	{
		if (ft_putchar_len(*str) == -1)
			return (-1);
		str++;
		len++;
	}
	return (len);
}
