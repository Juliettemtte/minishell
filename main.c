/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/09/20 13:18:16 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->cmd_list = NULL;
	variables->heredoc = ft_strdup("");
}

/*static void	free_var_token(t_var *var, t_token **tokens)
{
	int	i;

	if (var->input != NULL)
		free(var->input);
	i = 0;
	while (tokens[i] != NULL)
	{
		free(tokens[i]->value);
		if (tokens[i] != NULL)
			free(tokens[i]);
		i++;
	}
	if (tokens != NULL)
		free(tokens);
}*/

void	run_token_commands(t_token *token)
{
	int	i;
//	int	fd;
//	int	pid;

	i = 0;
/*	if (found_redirect)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = check_redirect2();
			dup2(fd, STDIN_FILENO);
		}
		else
		{
		}
	}*/
	while (token[i].value)
	{
		printf("found a token\n");
		printf("value = %s\n", token[i].value);
		if (token[i].type)
			printf("type = %d\n", token[i].type);
		i++;
	}
}

int	main(void)
{
	int		check;
	t_var	variables;
	t_token	*tokens;

	check = 0;
	initialise(&variables);
	init_signal();
	while (1)
	{
		variables.input = readline("prompt = ");
		if (variables.input == NULL)
		{
			//printf("caught EOF\n");
			//input = ft_strdup("EOF"); // temporary solution to ctrl-D
			exit(1);
		}
		check = parse(&variables); //return -1 for invalid, 1 for exit, 0 for valid
		if (check == -1)
			printf("invalid input\n");
		tokens = malloc(sizeof(t_token) * (count_cmd_list(variables.cmd_list) + 1));
		if (!tokens)
			return (1);
		tokenize_cmd_list(&variables, tokens);
//		if (check_command_syntax(tokens) == 1)
			//do not execute
		run_token_commands(tokens);
		if (variables.input)
		{
			add_history(variables.input);
			free(variables.input);
			//free_var_token(&variables, &tokens);
		}
		if (check == 1)
		{
			rl_clear_history();
			break ;
		}
	}
	//free_var_token(&variables, &tokens);
	return (0);
}
