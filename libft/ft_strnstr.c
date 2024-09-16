/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:05:14 by jmouette          #+#    #+#             */
/*   Updated: 2024/04/18 10:53:58 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (s2[j] == 0)
		return ((char *)s1);
	while (s1[i] != '\0' && i < len)
	{
		while (s1[i + j] == s2[j] && (i + j) < len && s1[i] != '\0')
		{
			if (s2[j + 1] == 0)
				return ((char *)s1 + i);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}
