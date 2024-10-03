/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:28:52 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/26 15:08:05 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_format(char *arg)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	if (!ft_isalpha(arg[0]))
		return (1);
	while (arg[i])
	{
		if (arg[i] == '=')
			equal = 1;
		else if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != '"')
			return (1);
		i++;
	}
	if (equal == 0)
		return (1);
	return (0);
}

int	check_quotes(char *input)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			single_quote++;
		if (input[i] == '\"')
			double_quote++;
		i++;
	}
	if (single_quote % 2 != 0 || double_quote % 2 != 0)
		return (1);
	return (0);
}
