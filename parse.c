/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/09/21 15:21:45 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_command2(t_var *var)
{
	char	**value;
	int		i;

	i = 7;
	if (is_builtins(var->cmd_list[0]) == 8)
	{
		if (!var->cmd_list[1])
			print_env_sorted();
		else
		{
			value = ft_split(var->cmd_list[1], '=');
			handle_export(value[0], value[1]);
			free_list(value);
		}
	}
	else if (is_builtins(var->cmd_list[0]) == 9)
	{
		value = ft_split(var->cmd_list[1], '=');
		handle_unset(value[0], ft_strlen(value[0]));
		free_list(value);
	}
	else
		find_cmd_path(var->cmd_list);
	return (0);
}

int	run_command(t_var *var)
{
	if (is_builtins(var->cmd_list[0]) == 1)
		return (1);
	if (is_builtins(var->cmd_list[0]) == 2)
		return (0);
	if (is_builtins(var->cmd_list[0]) == 3)
		return (handle_heredoc(var));
	if (is_builtins(var->cmd_list[0]) == 4)
		return (handle_pwd(var));
	if (is_builtins(var->cmd_list[0]) == 5)
		return (handle_echo(var->cmd_list));
	if (is_builtins(var->cmd_list[0]) == 6)
		handle_cd(var->cmd_list[1]);
	else if (is_builtins(var->cmd_list[0]) == 7)
		handle_env();
	else
		if (run_command2(var) == -1)
			return (-1);
	return (0);
}
/*
void	check_redirect(t_token **token)
{
	int	i;

	i = 0;
	while (token[i] && (token[i]-type == COMMAND || token[i]->type == ARGUMENT))
		i++;
	if (token[i] || token[i]->type == PIPE)
		return ;
	if (token[i]->type = REDIRECTION || token[i]->type == HEREDOC || token[i]->type == APPEND)

}*/

int	parse(t_var *var)
{
	int		check;
	int		i;

	if (check_quotes(var->input))
		return (1);
	var->cmd_list = split_input(var->input, ' ');
	if (!var->cmd_list)
		return (1);
	i = 0;
	check = 0;
	while (!var->cmd_list[i])
	{
		var->cmd_list[i] = ft_strtrim(var->cmd_list[i], "\'\"");
		if (!var->cmd_list[i])
			return (1);
		i++;
	}
	return (0);
}
