/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:40:11 by arissane          #+#    #+#             */
/*   Updated: 2024/10/09 17:49:31 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (str[i] == '\"')
				i++;
			count++;
		}
		else if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] == '\'')
				i++;
			count++;
		}
		else if (str[i] != c)
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
		else
			i++;
	}

	return (count);
}

static void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**split_input(char const *s, char c)
{
	char		**result;
	const char	*start;
	size_t		sub_len;
	int			i;
	int			inside_quotes;

	i = count_words(s, c);
	result = (char **)malloc((i + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			start = s;
			if (*s == '\"')
			{
				s++;
				while (*s && *s != '\"')
					s++;
				if (*s == '\"')
					s++;
			}
			if (*s == '\'')
			{
				s++;
				while (*s && *s != '\'')
					s++;
				if (*s == '\'')
					s++;
			}
			else
			{
			    inside_quotes = 0;
			    while (*s)
				{
			        if (*s == '\"' || *s == '\'')
			            inside_quotes = !inside_quotes;
			        if (*s == c && !inside_quotes)
			            break;
			        s++;
			    }
			}
			sub_len = s - start;
			//remove surrounding quotes
			//if ((start[0] == '\"' && start[sub_len - 1] == '\"') ||
			//		(start[0] == '\'' && start[sub_len - 1] == '\''))
			//{
			//	start++;
			//	sub_len += -2;
			//}
			result[i] = ft_substr(start, 0, sub_len);
			if (result[i] == NULL)
			{
				free_array(result);
				return (NULL);
			}
			i++;
		}
		else
			s++;
	}
	result[i] = 0;
	return (result);
}
