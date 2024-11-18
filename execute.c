/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:05:12 by arissane          #+#    #+#             */
/*   Updated: 2024/11/18 16:50:34 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execve_args(t_token **token, char *cmd_path, t_var *var, int size)
{
	char	**args;

	signal(SIGQUIT, SIG_DFL);
	while (token[size])
		size++;
	args = (char **)malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (0);
	args = fill_args(token, args);
	if (!args)
		return (1);
	if (execve(cmd_path, args, var->envp) == -1)
	{
		free(args);
		if (access(cmd_path, X_OK) != 0)
		{
			perror("Permission denied");
			exit(126);
		}
		perror("command not found");
		exit(127);
	}
	free(args);
	return (0);
}

static int	exec_parent_process(int pid)
{
	int	signal_number;
	int	status;

	signal(SIGQUIT, handle_sigquit_exec);
	signal(SIGINT, handle_sigint_exec);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failure");
		return (1);
	}
	if (WIFSIGNALED(status))
	{
		signal_number = WTERMSIG(status);
		if (signal_number == SIGSEGV)
		{
			ft_putstr_fd("Segmentation fault (core dumped)\n", 2);
			return (139);
		}
		return (128 + signal_number);
	}
	return (WEXITSTATUS(status));
}

static int	fork_exec(t_token **token, t_var *var, char *path, char *cmd)
{
	int	pid;

	if (var->fd_in != -1)
		close(var->fd_in);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
		return (-1);
	}
	if (pid == 0)
	{
		if (execve_args(token, path, var, 0) == -1)
		{
			handle_exec_errors(cmd);
			free(path);
			free_shell(var);
			exit(1);
		}
	}
	else
	{
		free(path);
		return (exec_parent_process(pid));
	}
	return (1);
}

int	execute_command(t_token **token_group, t_var *var, char *cmd)
{
	char	*cmd_path;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		cmd_path = ft_strdup(cmd);
		if (opendir(cmd_path) != NULL || access(cmd_path, F_OK) != 0)
		{
			free(cmd_path);
			return (handle_exec_errors(cmd));
		}
	}
	else
		cmd_path = find_cmd_path(cmd, 0, var);
	if (!cmd_path)
		return (handle_exec_errors(cmd));
	return (fork_exec(token_group, var, cmd_path, cmd));
}
