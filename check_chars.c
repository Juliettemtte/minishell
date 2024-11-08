/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:44:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/22 11:19:56 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_depth(char c, int *depth, char *outer_quote)
{
	if (*depth == 0)
	{
		*outer_quote = c;
		(*depth)++;
		return (0);
	}
	else if (c == *outer_quote && *depth == 1)
	{
		(*depth)--;
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
			if (check_depth(str[i], &depth, &outer_quote) == 0)
			{
				i++;
				continue ;
			}
		}
		str[j++] = str[i++];
	}
	while (str[j])
		str[j++] = '\0';
	return (str);
}

static char	*remove_single_quotes(t_var *var, char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			while (str[i + 1] != '\'')
				i++;
			i += 2;
		}
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' '
			&& str[i + 1] != '\"')
		{
			str = get_env_value(var, str, i);
		}
		i++;
	}
	return (remove_double_quotes(str, 0, 0));
}

void	check_characters(t_var *var, t_token **token_group)
{
	int	i;

	i = 0;
	while (token_group[i])
	{
		token_group[i]->value = remove_single_quotes(var,
				token_group[i]->value);
		i++;
	}
}
