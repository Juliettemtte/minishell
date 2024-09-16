/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:50:18 by jmouette          #+#    #+#             */
/*   Updated: 2024/04/22 13:51:33 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len_d;
	size_t	len_s;

	len_d = ft_strlen(dst);
	len_s = ft_strlen(src);
	if (len_d >= dstsize)
		len_d = dstsize;
	if (len_d == dstsize)
		return (dstsize + len_s);
	if (len_s < dstsize - len_d)
		ft_memcpy(dst + len_d, src, len_s + 1);
	else
	{
		ft_memcpy(dst + len_d, src, dstsize - len_d - 1);
		dst[dstsize - 1] = '\0';
	}
	return (len_d + len_s);
}
