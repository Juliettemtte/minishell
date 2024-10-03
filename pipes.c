/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:27:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/03 13:46:39 by jmouette         ###   ########.fr       */
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

static char	**execve_args(t_token **token_group)
{
	int		size;
	int		j;
	char	**args;

	size = 0;
	while (token_group[size])
		size++;
	args = (char **)malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (NULL);
	j = 0;
	while (token_group[j])
	{
		args[j] = token_group[j]->value;
		j++;
	}
	args[j] = NULL;
	return (args);
}

static int	execute_command(t_token **command_tokens)
{
	char	*cmd_path;
	char	**args;
	char	*command;

	command = command_tokens[0]->value;
	cmd_path = find_cmd_path(command);
	if (!cmd_path)
		return (-1);
	args = execve_args(command_tokens);
	if (!args)
	{
		free(cmd_path);
		return (-1);
	}
	if (execve(cmd_path, args, environ) == -1)
	{
		free(cmd_path);
		free(args);
		perror("execve");
		return (-1);
	}
	free(args);
	free(cmd_path);
	return (0);
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
			if (execute_cmd_tok(var, commands[i]) == 0)
				if (run_command(commands[i][0]->value, var, commands[i]) == 2)
					if (execute_command(commands[i]) == -1)
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

void	execute_pipes(t_var *var, t_token ***token_groups)
{
	int		i;
	int		pipefd[2];
	pid_t	pid;

	i = 0;
	while (i < var->pipes)
	{
		if (i < var->pipes - 1 && pipe(pipefd) == -1)
		{
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
		pid = fork();//fork processes
		if (pid < 0)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)//child process
		{
			if (i > 0)//if not the first command, redirect stdin to read end of the pipe
				dup2(pipefd[0], STDIN_FILENO);
			if (i < var->pipes - 1)//if not the last command, redirect stdout to write end of the pipe
				dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);//close both ends of the child process
			close(pipefd[1]);
			execute_cmd_tok(var, token_groups[i]);
			exit(EXIT_SUCCESS);
		}
		else//parent process
		{
			close(pipefd[1]);//parent process doesn't write
			if (i > 0)//if not the first command, close read
				close(pipefd[0]);
		}
		i++;
	}
	if (var->pipes > 1)
		close(pipefd[0]);//close read of last pipe
	i = 0;
	while (i < var->pipes)
	{
		wait(NULL);
		i++;
	}
}
