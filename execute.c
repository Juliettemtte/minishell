/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:05:12 by arissane          #+#    #+#             */
/*   Updated: 2024/10/19 15:53:14 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exec_errors(char *command)
{
	int	error_code;

	error_code = 127;
	if ((command[0] == '.' && command[1] == '/') || command[0] == '/')
	{
		if (access(command, F_OK) != 0)
		{
			error_code = 127;
			ft_putstr_fd(" No such file or directory\n", 2);
		}
		else if (opendir(command) != NULL)
		{
			error_code = 126;
			ft_putstr_fd(" Is a directory\n", 2);
		}
		else if (access(command, X_OK) != 0)
		{
			error_code = 126;
			ft_putstr_fd(" Permission denied\n", 2);
		}
	}
	else if ((command[0] != '<' && command[1] != '<')
		&& (command[0] != '>' && command[1] != '>'))
		ft_putstr_fd(" command not found\n", 2);
	return (error_code);
}

static char	**fill_args(t_token **token, char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (token[j])
	{
		if (token[j]->type > 4 && token[j]->type < 9)
			j++;
		else if (token[j]->type == 2 || token[j]->type == 1)
		{
			args[i] = token[j]->value;
			i++;
		}
		j++;
	}
	args[i] = NULL;
	return (args);
}

static int	execve_args(t_token **token, char *cmd_path, t_var *var)
{
	char	**args;
	int		size;

	size = 0;
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
		perror("command not found");
		free(args);
		exit(errno);
	}
	free(args);
	return (0);
}

static int	exec_child_parent(t_token **token, t_var *var, char *path, int pid)
{
	int	status;

	if (pid == 0)
	{
		g_status = EXECUTE;
		signal(SIGQUIT, handle_signal);
		if (execve_args(token, path, var) == -1)
		{
			handle_exec_errors(token[0]->value);
			free(path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free(path);
		return (WEXITSTATUS(status));
	}
	return (0);
}

int	execute_command(t_token **token_group, t_var *var)
{
	char	*cmd_path;
	pid_t	pid;

	if (token_group[0]->value[0] == '/')
	{
		cmd_path = strdup(token_group[0]->value);
		if (opendir(cmd_path) != NULL || access(cmd_path, F_OK) != 0)
		{
			free(cmd_path);
			return (handle_exec_errors(token_group[0]->value));
		}
	}
	else
		cmd_path = find_cmd_path(token_group[0]->value);
	if (!cmd_path)
		return (handle_exec_errors(token_group[0]->value));
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(cmd_path);
		return (-1);
	}
	return (exec_child_parent(token_group, var, cmd_path, pid));
}
