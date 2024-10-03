/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:15:50 by arissane          #+#    #+#             */
/*   Updated: 2024/10/03 13:32:49 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmd_tok(t_var *var, t_token **tokens)
{
	int	i;
/*
	i = 0;
	printf("\nfound tokens :\n");
	while (tokens[i]->value)
	{
		printf("token = %s\ntype = %d\n", tokens[i]->value, tokens[i]->type);
		i++;
	}
	write(1, "\n", 1);
	printf("pipes: %d\ncommands: %d\n", var->pipes, var->commands);
*/

	int	std_in;
	int	std_out;
	int	fd;
	int	check;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	check = 0;

	int	k = 0;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type > 4)//check redirects
			k++;
		i++;
	}

	//if (k == 0)
	//	var->status = run_command(tokens[0]->value, var, tokens);
	//else
	if (k > 0)
	{
		printf("redirecting...\n var->is_redirect == %d\n", var->is_redirect);
		i = 0;
		while (tokens[i] && var->status != 1)
		{
			if (tokens[i]->type == REDIRECTION_LEFT)
			{
				fd = open(tokens[i - 1]->value, O_RDONLY);
				if (fd < 0)
				{
					perror("Can't open file for reading");
					break ;
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
				check = 1;
			}
			if (tokens[i]->type == REDIRECTION_RIGHT)
			{
				fd = open(tokens[i + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd < 0)
				{
					perror("Can't open file for writing");
					break ;
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
				check = 1;
			}
			if (tokens[i]->type == APPEND)
			{
				fd = open(tokens[i + 1]->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd < 0)
				{
					perror("Can't open file for appending");
					break ;
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
				check = 1;
			}
			i++;
			if (check == 1)
				var->status = run_command(tokens[0]->value, var, tokens);
			check = 0;
		}
		//restore original stdin, stdout
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		close (std_in);
		close (std_out);
		return (1);
	}
	close (std_in);
	close (std_out);
	return (0);
}
