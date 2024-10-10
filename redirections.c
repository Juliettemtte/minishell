/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:43:11 by arissane          #+#    #+#             */
/*   Updated: 2024/10/10 17:46:05 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirect(t_var *var, t_token **tokens)
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


	if (k > 0)
	{
		i = 0;
		while (tokens[i] && var->exit_code != 1)
		{
			if (tokens[i]->type == REDIRECTION_LEFT)
			{
				fd = open(tokens[i + 1]->value, O_RDONLY);
				if (fd < 0)
				{
					perror("Can't open file for reading");
					break ;
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
				check = 1;
			}
			else if (tokens[i]->type == REDIRECTION_RIGHT)
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
			else if (tokens[i]->type == APPEND)
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
				var->exit_code = run_command(var, tokens);
			check = 0;
		}
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
