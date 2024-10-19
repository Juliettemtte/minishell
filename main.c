/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/10/19 16:33:39 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status = 0;

static void	initialise(t_var *variables, char **envp)
{
	variables->input = NULL;
	variables->cmd_list = NULL;
	variables->commands = 0;
	variables->pipes = 0;
	variables->is_redirect = 0;
	variables->input_redir = 0;
	variables->output_redir = 0;
	variables->heredoc_count = 0;
	variables->heredoc_fds = NULL;
	copy_env(variables, envp);
	init_signal();
}

static int	count_cmd(char **cmd_list)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (cmd_list[i])
	{
		if (ft_strcmp(cmd_list[i], "|") == 0)
			count++;
		i++;
	}
	return (count + 1);
}

int	main(int argc, char **argv, char **envp)
{
	int		check;
	int		exit_code;
	t_var	variables;
	t_token	*tokens;
	t_token	***token_groups;

	(void)argc;
	(void)argv;
	check = 0;
	variables.exit_code = 0;
	while (1)
	{
		initialise(&variables, envp);
		variables.input = readline("prompt = ");
		if (variables.input == NULL)
		{
			variables.input = ft_strdup("exit"); //ctrl-D behaves the same as manual exit, so that everything is freed correctly?
			//exit(EXIT_FAILURE);
		}
		add_history(variables.input);
		check = parse(&variables);
		if (check == -1)
			printf("invalid input\n");
		tokens = malloc(sizeof(t_token) * (count_cmd_list(variables.cmd_list) + 1));
		if (!tokens)
			return (1);
		tokenize_cmd_list(&variables, tokens);
		if (handle_heredoc(&variables, tokens) == 1)
		{
			printf("heredoc error\n");
			return (1);
		}
		token_groups = split_tokens(&variables, tokens);
		if (variables.pipes == 0 && variables.is_redirect == 0)
			variables.exit_code = run_command(&variables, token_groups[0]);
		else
		{
			variables.nb_cmd = count_cmd(variables.cmd_list);
			handle_pipe(token_groups, variables.nb_cmd, &variables);
		}
		close_heredoc_fds(&variables);
		g_status = 0;
		if (token_groups)
		{
			if (check == -2 || variables.exit_code == -2)
				exit_code = my_exit(token_groups[0]);
			free_shell(&variables, tokens, token_groups);
		}
		if (check == -2 || variables.exit_code == -2)
		{
			rl_clear_history();
			break ;
		}
	}
	clear_history();
	exit (exit_code);
	return (0);
}
