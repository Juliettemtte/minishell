/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:49:09 by arissane          #+#    #+#             */
/*   Updated: 2024/11/15 09:48:30 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//tokenise the input that has been parsed into a char *array,
//check if there are heredocs and allocate space for them, then
//check if pipes or redirections exist and either run a single
//command line, or go to handle_pipe for redirection and pipe handling
static int	handle_commands(t_var *var)
{
	var->tokens = malloc(sizeof(t_token) * (count_cmd_list(var->cmd_list) + 1));
	if (!var->tokens)
		return (1);
	init_heredoc_fds(var);
	if (tokenize_cmd_list(var, var->tokens) == 2)
	{
		var->exit_code = 2;
		return (3);
	}
	if (handle_heredoc(var, var->tokens) != 1)
	{
		var->token_groups = split_tokens(var, var->tokens);
		if (var->pipes == 0 && var->is_redirect == 0)
			var->exit_code = run_command(var, var->token_groups[0]);
		else
		{
			var->nb_cmd = count_cmd(var->cmd_list);
			handle_pipe(var->token_groups, var->nb_cmd, var);
		}
	}
	else
		return (2);
	return (0);
}

static int	parse_and_execute_helper(t_var *var)
{
	int	check;

	check = parse(var);
	if (check == 2)
	{
		var->exit_code = 2;
		return (2);
	}
	if (check == 3)
		return (0);
	if (check == 1)
		return (1);
	check = handle_commands(var);
	if (check == 2)
		dup2(var->fd_in, STDIN_FILENO);
	close_heredoc_fds(var);
	return (check);
}

//if input is only enter, don't add it to history just
//like bash, otherwise continue to parsing and update exit
//codes if signals were received
void	parse_and_execute(t_var *var)
{
	if (var->input[0] != '\0')
		add_history(var->input);
	if (parse_and_execute_helper(var) == 1)
	{
		ft_putstr_fd("malloc error\n", 2);
		var->exit_code = 1;
	}
	if (g_signal == SIGINT)
		var->exit_code = 130;
	if (g_signal == SIGQUIT)
		var->exit_code = 131;
	g_signal = 0;
}
