/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:55:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/03 17:27:01 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *arg)
{
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		exit(EXIT_FAILURE);
	for (int i = 1; arg[i] != '\0'; i++)
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			exit(EXIT_FAILURE);
	return (0);
}

static char	**is_valid(t_var *var)
{
	int		i;
	char	*equal_sign;
	char	*cmd;

	i = 0;
	while (var->cmd_list[i] != NULL)
	{
		cmd = var->cmd_list[i];
		if (ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0)
		{
			i++;
			continue;
		}
		if (!is_valid_identifier(cmd))
			exit(0);
		equal_sign = ft_strchr(cmd, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			return ft_split(cmd, '=');
		}
		else
			return ft_split(cmd, '=');
	}
	return (NULL);
}

static int	no_redirect(t_var *var)
{
	char	*path;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		path = find_cmd_path(var->cmd_list[0]);
		if (!path)
			exit(EXIT_FAILURE);
		if (execve(path, var->cmd_list, environ) == -1)
		{
			free (path);
			exit(EXIT_FAILURE);
		}
		free(path);
		exit(EXIT_SUCCESS);
	}
	else
		waitpid(pid, &status, 0);
	return (0);
}

static int	run_command2(char *cmd, t_var *var)
{
	char	**value;

	if (is_builtins(cmd) == 7)
	{
		if (!var->cmd_list[1])
			print_env_sorted();
		else
		{
			value = is_valid(var);
			if (!value)
				exit(EXIT_FAILURE);
			handle_export(value[0], value[1]);
			free_list(value);
		}
		exit(EXIT_SUCCESS);
	}
	else if (is_builtins(cmd) == 8)
	{
		value = is_valid(var);
		if (!value)
			exit(EXIT_FAILURE);
		handle_unset(value[0], ft_strlen(value[0]));
		free_list(value);
		exit(EXIT_SUCCESS);
	}
	else if (var->is_redirect == 0)
		if (no_redirect(var) == -1)
			return (-1);
	return (2);
}

int	run_command(char *cmd, t_var *var, t_token **token_group)
{
	if (is_builtins(cmd) == 1)
		return (my_exit((char **)token_group));
	if (is_builtins(cmd) == 2)
		return (0);
	if (is_builtins(cmd) == 9)
		return (handle_heredoc(var));
	if (is_builtins(cmd) == 3)
		return (handle_pwd(var));
	if (is_builtins(cmd) == 4)
		return (handle_echo(token_group));
	if (is_builtins(cmd) == 5)
		return(handle_cd(token_group));
	else if (is_builtins(cmd) == 6)
	{
		handle_env();
		exit(EXIT_SUCCESS);
	}
	return (run_command2(cmd, var));
}
