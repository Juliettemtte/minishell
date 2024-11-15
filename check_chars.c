/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:44:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/15 11:48:41 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_inside_quotes(char c, int *depth, char *outer_quote)
{
	if (*depth == 0)
	{
		*outer_quote = c;
		*depth = 1;
		return (0);
	}
	else if (c == *outer_quote && *depth == 1)
	{
		*depth = 0;
		*outer_quote = '\0';
		return (0);
	}
	return (1);
}

static char	*remove_double_quotes(char *str, int i, int j)
{
	int		depth;
	char	outer_quote;

	depth = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (is_inside_quotes(str[i], &depth, &outer_quote) == 0)
			{
				i++;
				continue ;
			}
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
	return (str);
}

char	*remove_quotes(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] == '\'')
				i++;
		}
		else
			i++;
	}
	return (remove_double_quotes(str, 0, 0));
}

void	check_characters(t_token **token_group)
{
	int		i;

	i = 0;
	while (token_group[i])
	{
		token_group[i]->value = remove_quotes(
				token_group[i]->value);
		i++;
	}
}
