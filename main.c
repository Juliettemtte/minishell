/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/09/16 18:18:02 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->cmd_list = NULL;
	variables->heredoc = ft_strdup("");
}

static void	ft_free(t_var *var, t_token **tokens)
{
	int	i;

	free(var->input);
	free(var);
	free_list(var->cmd_list);
	i = 0;
	while (tokens[i] != NULL)
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
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
			return ;
		tokenize_cmd_list(&variables, &tokens);
		if (variables.input)
		{
			add_history(variables.input);
			ft_free(&variables, &tokens);
		}
		if (check == 1)
		{
			rl_clear_history();
			break ;
		}
	}
	ft_free(&variables, &tokens);
	return (0);
}
