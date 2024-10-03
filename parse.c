/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/10/02 17:09:43 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_var *var)
{
	int		i;

	if (check_quotes(var->input))
		return (1);
	var->cmd_list = split_input(var->input, ' ');
	if (!var->cmd_list)
		return (1);
	i = 0;
	while (var->cmd_list[i])
	{
		var->cmd_list[i] = ft_strtrim(var->cmd_list[i], "\'\"");
		if (!var->cmd_list[i])
			return (1);
		if (is_builtins(var->cmd_list[i]) >= 9)
			var->is_redirect = 1;
		if (is_builtins(var->cmd_list[i]) == 9) //check the number of pipes. If there is no valid argument after the pipe, return
		{
			if ((var->cmd_list[i + 1] && var->cmd_list[i + 1][0] == '|') || !var->cmd_list[i + 1])
				return (-1);
			var->pipes++;
		}
		i++;
	}
	return (0);
}
