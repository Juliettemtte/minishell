/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:32:00 by jmouette          #+#    #+#             */
/*   Updated: 2024/04/18 10:35:56 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char	*ps1;
	char	*ps2;
	size_t	i;

	ps1 = (char *)s1;
	ps2 = (char *)s2;
	i = 0;
	while (i < n)
	{
		if (ps1[i] != ps2[i])
			return ((unsigned char)ps1[i] - (unsigned char)ps2[i]);
		i++;
	}
	return (0);
}
