/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:28:52 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/20 13:34:20 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_format(char *arg)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	if (!ft_isalpha(arg[0]))
		return (1);
	while (arg[i])
	{
		if (arg[i] == '=')
			equal = 1;
		else if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != '"')
			return (1);
		i++;
	}
	if (equal == 0)
		return (1);
	return (0);
}

int	check_quotes(char *input)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			single_quote++;
		if (input[i] == '\"')
			double_quote++;
		i++;
	}
	if (single_quote % 2 != 0 || double_quote % 2 != 0)
		return (1);
	return (0);
}

int	check_command_syntax(t_token *tokens)
{
	int		current_type;
	int		next_type;
	int				i;

	i = 0;
	if (tokens[0].type != COMMAND)
	{
		printf("Syntax Error: Command expected at the beginning\n");
		return (1);
	}
	while (tokens[i].value != NULL)
	{
		current_type = tokens[i].type;
        if (tokens[i + 1].value != NULL)
		{
			next_type = tokens[i + 1].type;
			if (current_type == PIPE && next_type != COMMAND)
			{
				printf("Syntax Error: Command expected after PIPE\n");
				return (1);
			}
			if ((current_type == REDIRECTION_LEFT || current_type == REDIRECTION_RIGHT
				|| current_type == APPEND) && next_type != ARGUMENT)
			{
				printf("Syntax Error: Filename expected after redirection\n");
				return (1);
			}
		}
		else if (current_type == PIPE)
		{
                printf("Syntax Error: Command expected after PIPE\n");
                return (1);
        }
		else if(current_type == REDIRECTION_LEFT || current_type == REDIRECTION_RIGHT
				|| current_type == APPEND)
		{
				printf("Syntax Error: Filename expected after redirection\n");
				return (1);
		}
		i++;
	}
	return (0);
}
