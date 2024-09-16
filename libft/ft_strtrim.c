/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 09:38:13 by jmouette          #+#    #+#             */
/*   Updated: 2024/04/23 09:36:49 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	const char	*end;
	char		*str;
	size_t		size;

	if (!s1 || !set)
		return (0);
	while (*s1 && ft_strchr(set, *s1) != 0)
		s1++;
	end = s1 + ft_strlen(s1) - 1;
	while (end > s1 && ft_strchr(set, *end) != 0)
		end--;
	size = (end - s1) + 1;
	str = (char *)malloc((size + 1) * sizeof(char));
	if (!str)
		return (0);
	ft_memcpy(str, s1, size);
	str[size] = '\0';
	return (str);
}
