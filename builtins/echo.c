/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:08:47 by arissane          #+#    #+#             */
/*   Updated: 2024/11/21 15:23:04 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_exceptions(t_token **token_group, int i)
{
	if (token_group[i] && token_group[i + 1])
	{
		if (token_group[i]->type == REDIRECTION_RIGHT)
			return (0);
		if (token_group[i]->type == REDIRECTION_LEFT)
			return (0);
		if (token_group[i]->type == APPEND)
			return (0);
		if (token_group[i]->type == HEREDOC)
			return (0);
	}
	return (1);
}

static int	print_string(t_token **token, int i)
{
	int	k;

	k = 0;
	while (token[i]->value[k])
	{
		write(1, &token[i]->value[k], 1);
		k++;
	}
	if (token[i + 1])
		write(1, " ", 1);
	i++;
	return (i);
}

static int	check_n_syntax(char *str)
{
	int	i;

	if (str[0] == '-')
		i = 1;
	else
		return (1);
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	handle_echo(t_token **token)
{
	int	i;
	int	check_n;

	i = 1;
	check_n = 0;
	if (token[i] && check_n_syntax(token[i]->value) == 0)
	{
		check_n = 1;
		i++;
	}
	while (token[i] && check_n_syntax(token[i]->value) == 0)
		i++;
	while (token[i])
	{
		if (check_exceptions(token, i) == 0)
		{
			i += 2;
			continue ;
		}
		if (token[i])
			i = print_string(token, i);
	}
	if (check_n == 0)
		write(1, "\n", 1);
	return (0);
}
