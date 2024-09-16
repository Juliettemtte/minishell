/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 11:31:44 by jmouette          #+#    #+#             */
/*   Updated: 2024/07/05 14:55:37 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_copy_to_stash(char *stash, char *buffer)
{
	char	*temp;
	char	*res;

	res = 0;
	if (!stash && buffer)
	{
		res = ft_strdup_gnl(buffer);
		if (!res)
			return (ft_free(&res, 0));
		return (res);
	}
	temp = ft_strdup_gnl(stash);
	if (!temp)
	{
		ft_free(&stash, 0);
		return (ft_free(&temp, 0));
	}
	ft_free(&stash, 0);
	res = ft_strjoin_gnl(temp, buffer);
	if (!res)
		ft_free(&res, 0);
	ft_free(&temp, 0);
	return (res);
}

static int	ft_have_nl(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_extract_line(char *stash)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	if (!stash)
		return (ft_free(&stash, 0));
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (ft_free(&line, 0));
	j = 0;
	while (j < i + 1)
	{
		line[j] = stash[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*ft_recreate_stash(char *stash)
{
	size_t	i;
	char	*res;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n')
		i++;
	if (stash[i + 1] == '\0')
		return (ft_free(&stash, 0));
	res = ft_substr_gnl(stash, i + 1, ft_strlen_gnl(stash));
	if (!res)
	{
		ft_free(&stash, 0);
		return (NULL);
	}
	ft_free(&stash, 0);
	return (res);
}

char	*get_next_line(int fd)
{
	char		buffer[BUFFER_SIZE + 1];
	long		ret;
	static char	*stash = NULL;
	char		*line;

	line = 0;
	ret = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (ft_free(&stash, 0));
	while (ret > 0)
	{
		ret = read(fd, buffer, BUFFER_SIZE);
		if (ret <= 0 && !stash)
			return (ft_free(&stash, 0));
		buffer[ret] = '\0';
		stash = ft_copy_to_stash(stash, buffer);
		if (ft_have_nl(stash))
		{
			line = ft_extract_line(stash);
			if (!line)
				return (ft_free(&stash, 0));
			return (stash = ft_recreate_stash(stash), line);
		}
	}
	return (ft_free(&stash, 1));
}
