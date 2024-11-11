/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:14:55 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/10 10:45:13 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	number_size(long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		i;
	int		sign;
	long	nbr;

	nbr = n;
	sign = 0;
	if (nbr < 0)
	{
		nbr = -nbr;
		sign = 1;
	}
	i = number_size(nbr) + sign - 1;
	result = (char *)malloc((number_size(nbr) + 1 + sign) * sizeof(char));
	if (!result)
		return (0);
	result[i + 1] = '\0';
	while (i >= 0)
	{
		result[i--] = (nbr % 10) + 48;
		nbr = nbr / 10;
	}
	if (sign == 1)
		result[0] = '-';
	return (result);
}
