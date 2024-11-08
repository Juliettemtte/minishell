/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/11/08 13:29:22 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_cmd_list(t_var *var)
{
	int	i;

	i = 0;
	while (var->cmd_list[i])
	{
		if (is_builtins(var->cmd_list[i]) >= 9)
			var->is_redirect = 1;
		if (is_builtins(var->cmd_list[i]) == 9)
		{
			if ((var->cmd_list[i + 1] && var->cmd_list[i + 1][0] == '|')
				|| !var->cmd_list[i + 1])
				return (2);
			var->pipes++;
		}
		if (is_builtins(var->cmd_list[i]) == 10)
			var->heredoc_count++;
		i++;
	}
	return (0);
}

int	parse(t_var *var)
{
	if (split_redirections(var) == 1)
		return (2);
	var->cmd_list = split_input(var->str, ' ');
	free(var->str);
	if (!var->cmd_list)
		return (1);
	return (parse_cmd_list(var));
}
