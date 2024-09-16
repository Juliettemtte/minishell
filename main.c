/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:42:58 by arissane          #+#    #+#             */
/*   Updated: 2024/09/16 17:52:21 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_var *variables)
{
	variables->input = NULL;
	variables->cmd_list = NULL;
	variables->heredoc = ft_strdup("");
}

int	main(void)
{
	int		check;
	t_var	variables;

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
		tokenize_cmd_list(&variables);
		if (variables.input)
		{
			add_history(variables.input);
			free(variables.input);
		}
		if (check == 1)
		{
			rl_clear_history();
			break ;
		}
	}
	return (0);
}
