/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/11/15 09:43:08 by arissane         ###   ########.fr       */
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

//ctrl-C and ctrl-D both cause the readline input to be null
//if ctrl-C, restore stdin after it was closed in the signal
//function. Only write a newline if it's the first call or if
//the previous call was just enter
static void	handle_null_input(t_var *var)
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

static int	run_shell(t_var *var, int exit_code)
{
	while (1)
	{
		initialise(var);
		var->input = readline("prompt = ");
		if (var->input == NULL)
			handle_null_input(var);
		else
			parse_and_execute(var);
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
	exit_code = run_shell(&variables, 0);
	if (variables.envp)
		free_env(&variables.envp);
	rl_clear_history();
	return (exit_code);
}
