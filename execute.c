/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:05:12 by arissane          #+#    #+#             */
/*   Updated: 2024/10/10 17:03:04 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exec_errors(char *command)
{
	int	error_code;

	error_code = 0;
	if ((command[0] == '.' && command[1] == '/') || command[0] == '/')
	{
		if (errno == EISDIR || errno == EACCES)
		{
			error_code = 126;
			if (access(command, R_OK | W_OK | X_OK) != 0)
				ft_putstr_fd(" Permission denied\n", 2);
			else
				ft_putstr_fd(" Is a directory\n", 2);
		}
		else if (errno == ENOENT)
		{
			error_code = 127;
			ft_putstr_fd(" No such file or directory\n", 2);
		}
	}
	else
	{
		error_code = 127;
		ft_putstr_fd(" command not found\n", 2);
	}
	return (error_code);
}

int	execve_args(t_token **token, char *cmd_path)
{
	int		size;
	int		j;
	char	**args;

	size = 0;
	while (token[size])
		size++;
	args = (char **)malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (1);
	j = 0;
	while (token[j] && (token[j]->type == 2 || token[j]->type == 1))
	{
		args[j] = token[j]->value;
		j++;
	}
	args[j] = NULL;
	if (execve(cmd_path, args, environ) == -1)
	{
		free(args);
		perror("command not found");
		return (errno);
	}
	free (args);
	return (0);
}

int	execute_command(t_token **token_group)
{
	char	*cmd_path;
	char	**args;
	char	*command;

	args = NULL;
	command = token_group[0]->value;
	cmd_path = find_cmd_path(command);
	if (!cmd_path)
	{
		if (execve(command, args, environ) == -1)
			return (handle_exec_errors(command));
		return (0);
	}
	if (!execve_args(token_group, cmd_path))
	{
		free(cmd_path);
		return (1);
	}
	free(cmd_path);
	return (0);
}
