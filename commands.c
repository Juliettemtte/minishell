/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:55:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/15 13:01:15 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_command(t_token **token_group)
{
	int		i;

	if (!token_group)
		return (NULL);
	i = 0;
	while (token_group[i] && token_group[i]->value && token_group[i]->type > 2)
	{
		if (token_group[i]->type != 6)
			i++;
		i++;
	}
	if (token_group[i])
		return (token_group[i]->value);
	return (NULL);
}

int	run_command(t_var *var, t_token **token_group)
{
	char	*cmd;

	if (!token_group)
		return (0);
	check_characters(token_group);
	cmd = find_command(token_group);
	if (!cmd)
		return (0);
	if (is_builtins(cmd) == 1)
		return (-2);
	if (is_builtins(cmd) == 2)
		return (0);
	if (is_builtins(cmd) == 3)
		return (handle_pwd(var));
	if (is_builtins(cmd) == 4)
		return (handle_echo(token_group));
	if (is_builtins(cmd) == 5)
		return (handle_cd(token_group, var));
	if (is_builtins(cmd) == 6)
		return (handle_env(var, token_group));
	if (is_builtins(cmd) == 7)
		return (handle_export(token_group, var));
	if (is_builtins(cmd) == 8)
		return (handle_unset(token_group, var));
	return (execute_command(token_group, var, cmd));
}
