/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:40:11 by arissane          #+#    #+#             */
/*   Updated: 2024/09/10 16:28:24 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s && s)
	{
		if (*s == '"' || *s == '\'')
		{
			s++;
			while (*s && *s != '"' && *s != '\'')
			{
				s++;
				if (*s == '$')
					count++;
			}
			count++;
		}
		else if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

static void	*free_array(char ***result, int i)
{
	while (--i >= 0)
	{
		free((*result)[i]);
		(*result)[i] = NULL;
	}
	free(*result);
	(*result) = NULL;
	return (0);
}

char	**split_input(char const *s, char c)
{
	char		**result;
	int			i;
	const char	*start;

	result = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	i = 0;
	if (!result)
		return (0);
	while (*s)
	{
		if (*s != c)
		{
			start = s;
			if (*s == '"' || *s == '\'')
			{
				s++;
				while (*s && *s != '"' && *s != '\'' && *s != '$')
					s++;
			}
			else
				while (*s && *s != c)
					s++;
			result[i] = ft_substr(start, 0, s - start);
			if (result[i] == NULL)
				return (free_array(&result, i));
			i++;
		}
		s++;
	}
	result[i] = 0;
	return (result);
}
