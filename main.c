/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/11/13 15:56:08 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->str = NULL;
	variables->cmd_list = NULL;
	variables->tokens = NULL;
	variables->token_groups = NULL;
	variables->commands = 0;
	variables->pipes = 0;
	variables->is_redirect = 0;
	variables->input_redir = 0;
	variables->output_redir = 0;
	variables->heredoc_count = 0;
	variables->heredoc_fds = NULL;
	variables->fd_in = dup(STDIN_FILENO);
	init_signal();
}

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

static int	parse_and_execute(t_var *var)
{
	int	check;

	check = parse(var);
	if (check == 2)
	{
		var->exit_code = 2;
		write(2, "syntax error\n", 13);
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

static int	run_shell(t_var *var)
{
	int		exit_code;

	while (1)
	{
		initialise(var);
		var->input = readline("prompt = ");
		if (var->input == NULL)
		{
			if (g_signal == SIGINT)
			{
				dup2(var->fd_in, STDIN_FILENO);
				if (var->exit_code != 130 || var->status == EMPTY_LINE)
				{
					var->status = 0;
					write(1, "\n", 1);
				}
				var->exit_code = 130;
				g_signal = 0;
			}
			else
				var->exit_code = -2;
		}
		else
		{
			add_history(var->input);
			if (parse_and_execute(var) == 1)
			{
				write(2, "malloc error\n", 13);
				var->exit_code = 1;
			}
			if (g_signal == SIGINT)
				var->exit_code = 130;
			if (g_signal == SIGQUIT)
				var->exit_code = 131;
			g_signal = 0;
		}
		close(var->fd_in);
		if (var->exit_code == -2)
		{
			exit_code = 1;
			if (var->token_groups != NULL)
				exit_code = my_exit(var->token_groups[0]);
			if (exit_code != -3)
			{
				free_shell(var);
				write(1, "exit\n", 5);
				return (exit_code);
			}
			var->exit_code = 1;
		}
		free_shell(var);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_code;
	t_var	variables;

	(void)argv;
	if (argc != 1)
	{
		printf("Too many arguments\n");
		return (1);
	}
	exit_code = 1;
	variables.status = 0;
	variables.exit_code = 0;
	copy_env(&variables, envp);
	exit_code = run_shell(&variables);
	if (variables.envp)
		free_env(&variables.envp);
	rl_clear_history();
	return (exit_code);
}
