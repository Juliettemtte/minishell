/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:40:11 by arissane          #+#    #+#             */
/*   Updated: 2024/11/13 15:59:26 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words_in_quotes(char const *str, int *i)
{
	int		count;
	char	quote_char;

	count = 0;
	if (str[*i] == '\"' || str[*i] == '\'')
	{
		quote_char = str[*i];
		(*i)++;
		while (str[*i] && str[*i] != quote_char)
			(*i)++;
		if (str[*i] == quote_char)
			(*i)++;
		count++;
	}
	return (count);
}

static int	count_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		count += count_words_in_quotes(str, &i);
		if (str[i] != c)
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

static const char	*find_next_token(const char *str, char c, int *in_quotes)
{
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			*in_quotes = !(*in_quotes);
		if (*str == c && !(*in_quotes))
			break ;
		str++;
	}
	return (str);
}

static char	*extract_token(const char **str, char c)
{
	const char	*start;
	char		quote_char;
	int			inside_quotes;

	start = *str;
	inside_quotes = 0;
	if (**str == '\"' || **str == '\'')
	{
		quote_char = **str;
		(*str)++;
		while (**str && **str != quote_char)
			(*str)++;
		if (**str == quote_char)
			(*str)++;
	}
	*str = find_next_token(*str, c, &inside_quotes);
	return (ft_substr(start, 0, *str - start));
}

char	**split_input(char const *str, char c)
{
	char	**result;
	int		i;

	i = count_words(str, c);
	result = (char **)malloc((i + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			result[i] = extract_token(&str, c);
			if (!result[i])
			{
				free_list(result);
				return (NULL);
			}
			i++;
		}
		else
			str++;
	}
	result[i] = NULL;
	return (result);
}
