/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:05:12 by arissane          #+#    #+#             */
/*   Updated: 2024/11/13 15:54:07 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_exec_errors(char *command)
{
	int	error_code;

	error_code = 0;
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
		error_code = validate_cmd_path(command);
	else if ((command[0] != '<' && command[1] != '<')
		&& (command[0] != '>' && command[1] != '>'))
	{
		error_code = 127;
		ft_putstr_fd(" command not found\n", 2);
	}
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

	signal(SIGQUIT, SIG_DFL);
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
		free(args);
		if (access(cmd_path, X_OK) != 0)
		{
			perror("Permission denied");
			exit(126);
		}
		perror("command not found");
		exit(errno);
	}
	free(args);
	return (0);
}

static int	fork_exec(t_token **token, t_var *var, char *path, int pid)
{
	int	status;
	int	signal_number;

	if (pid == 0)
	{
		if (execve_args(token, path, var) == -1)
		{
			handle_exec_errors(token[0]->value);
			free(path);
			exit(1);
		}
	}
	else
	{
		signal(SIGQUIT, handle_sigquit_exec);
		signal(SIGINT, handle_sigint_exec);
		free(path);
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
	return (1);
}

int	execute_command(t_token **token_group, t_var *var)
{
	char	*cmd_path;
	pid_t	pid;

	if (token_group[0]->value[0] == '/' || token_group[0]->value[0] == '.')
	{
		cmd_path = strdup(token_group[0]->value);
		if (opendir(cmd_path) != NULL || access(cmd_path, F_OK) != 0)
		{
			free(cmd_path);
			return (handle_exec_errors(token_group[0]->value));
		}
	}
	else
		cmd_path = find_cmd_path(token_group[0]->value, 0, var);
	if (!cmd_path)
		return (handle_exec_errors(token_group[0]->value));
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(cmd_path);
		return (-1);
	}
	return (fork_exec(token_group, var, cmd_path, pid));
}
