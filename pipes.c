/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:27:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/11 18:50:41 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_redirect(int i, int nb_cmd, int prev_pipefd[2], int pipefd[2])
{
	if (i > 0)
	{
		if (dup2(prev_pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		close(prev_pipefd[0]);
		close(prev_pipefd[1]);
	}
	if (i < nb_cmd - 1)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void	handle_pipe(t_token **commands[], int num_commands, t_var *var)
{
	int		i;
	int		pipefd[2];
	int		prev_pipefd[2];
	pid_t	pid;

	i = 0;
	while (i < num_commands)
	{
		if (i < num_commands - 1 && pipe(pipefd) == -1)
			exit(EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0)
		{
			setup_redirect(i, num_commands, prev_pipefd, pipefd);
			if (handle_redirect(var, commands[i]) == 0)
			{
				if (run_command(var, commands[i]) == 2)
					exit(EXIT_FAILURE);
			}
			else
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (i > 0)
			{
				close(prev_pipefd[0]);
				close(prev_pipefd[1]);
			}
			if (i < num_commands - 1)
			{
				prev_pipefd[0] = pipefd[0];
				prev_pipefd[1] = pipefd[1];
			}
		}
		i++;
	}
	i = 0;
	while (i < num_commands)
	{
		wait(NULL);
		i++;
	}
}
