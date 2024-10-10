/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/10/10 17:38:30 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->cmd_list = NULL;
	variables->heredoc = ft_strdup("");
	variables->commands = 0;
	variables->pipes = 0;
	variables->is_redirect = 0;
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

int	main(int argc, char **argv)
{
	int		check;
	int		exit_code;
	t_var	variables;
	t_token	*tokens;
	t_token	***token_groups;

	(void)argc; //It seems we are suposed to ignore additional
	(void)argv; //arguments when we run the minishell
	check = 0;
	init_signal();
	while (1)
	{
		initialise(&variables);
		variables.input = readline("prompt = ");
		if (variables.input == NULL)
		{
			//printf("caught EOF\n");
			//input = ft_strdup("EOF"); // temporary solution to ctrl-D
			exit(EXIT_FAILURE);
		}
		add_history(variables.input);
		check = parse(&variables); //return -1 for invalid, -2 for exit, 0 for valid
		if (check == -1)
			printf("invalid input\n");
		tokens = malloc(sizeof(t_token) * (count_cmd_list(variables.cmd_list) + 1));
		if (!tokens)
			return (1);
		tokenize_cmd_list(&variables, tokens);
		token_groups = split_tokens(&variables, tokens);
//for testing:		run_command(token_groups[0][0]->value, &variables, token_groups[0]);
//		execute_cmd_tok(&variables, token_groups[0]);
		if (variables.pipes == 0 && variables.is_redirect == 0)
		{
			check = run_command(&variables, token_groups[0]);
			variables.exit_code = check;
		}
		else
		{
			variables.nb_cmd = count_cmd(variables.cmd_list);
			handle_pipe(token_groups, variables.nb_cmd, &variables);
		}
		if (variables.input)
			free(variables.input);
		if (token_groups)
		{
			if (check == -2 || variables.exit_code == -2)
				exit_code = my_exit(token_groups[0]);
			free_token_groups(token_groups);
		}
		if (variables.cmd_list)
			free_list(variables.cmd_list);
		if (check == -2 || variables.exit_code == -2)
		{
			rl_clear_history();
			break ;
		}
	}
	if (variables.heredoc)
		free(variables.heredoc);
	exit (exit_code);
	return (0);
}
