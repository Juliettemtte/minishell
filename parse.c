/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/09/16 17:46:48 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_var *var)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (strcmp(line, var->cmd_list[1]) == 0)
			break ;
		var->heredoc = ft_strjoin(var->heredoc, line);
	}
	if (var->cmd_list[1] && var->cmd_list[2] && var->cmd_list[3])
	{
		if (ft_strncmp(var->cmd_list[2], ">", 2) == 0)
			redirectreplace_output_right(var->heredoc, var->cmd_list[3]);
		else if (ft_strncmp(var->cmd_list[2], ">>", 3) == 0)
			redirect_output_right(var->heredoc, var->cmd_list[3]);
		else
			return (-1);
	}
	else
	{
		printf("%s\n", var->heredoc);
	}
	return (0);
}

//get the current working directory with cwd and print it out, or write it in a file after > / >>
int	handle_pwd(t_var *var)
{
	char	*pwd_content;

	pwd_content = getcwd(NULL, 0);
	if (var->cmd_list[1] && var->cmd_list[2])
	{
		if (ft_strncmp(var->cmd_list[1], ">", 2) == 0)
			redirectreplace_output_right(pwd_content, var->cmd_list[2]);
		else if (ft_strncmp(var->cmd_list[1], ">>", 3) == 0)
			redirect_output_right(pwd_content, var->cmd_list[2]);
		else
			return (-1);
	}
	else
		printf("%s\n", pwd_content);
	return (0);
}

//check if > or >> are called, otherwise if there is no input after echo, write a newline, or if there is input, print all of it
int	handle_echo(t_var *var)
{
	int	i;

	i = 1;
	if (ft_strcmp(var->cmd_list[1], "-n") == 0)
	{
		i++;
		while (var->cmd_list[i + 1])
		{
			printf("%s ", var->cmd_list[i]);
			i++;
		}
		printf("%s", var->cmd_list[i]);
	}
	else if (var->cmd_list[1] && var->cmd_list[2] && var->cmd_list[3])
	{
		if (ft_strncmp(var->cmd_list[2], ">", 2) == 0)
			redirectreplace_output_right(var->cmd_list[1], var->cmd_list[3]);
		else if (ft_strncmp(var->cmd_list[2], ">>", 3) == 0)
			redirect_output_right(var->cmd_list[1], var->cmd_list[3]);
		else
			return (-1);
	}
	else if (!var->cmd_list[1])
		write(1, "\n", 1);
	else
	{
		while (var->cmd_list[i + 1])
		{
			printf("%s ", var->cmd_list[i]);
			i++;
		}
		printf("%s\n", var->cmd_list[i]);
	}
	return (0);
}

int	check_command2(t_var *var)
{
	char	**value;
	int		i;

	i = 7;
	if (is_builtins(var->cmd_list[0]) == 8)
	{
		if (!var->cmd_list[1])
			print_env_sorted();
		else
		{
			value = ft_split(var->cmd_list[1], '=');
			handle_export(value[0], value[1]);
			free_list(value);
		}
	}
	else if (is_builtins(var->cmd_list[0]) == 9)
	{
		value = ft_split(var->cmd_list[1], '=');
		handle_unset(value[0], ft_strlen(value[0]));
		free_list(value);
	}
	else
		return (-1);
	return (0);
}

int	check_command(t_var *var)
{
	if (is_builtins(var->cmd_list[0]) == 1)
		return (1);
	if (is_builtins(var->cmd_list[0]) == 2)
		return (0);
	if (is_builtins(var->cmd_list[0]) == 3)
		return (handle_heredoc(var));
	if (is_builtins(var->cmd_list[0]) == 4)
		return (handle_pwd(var));
	if (is_builtins(var->cmd_list[0]) == 5)
		return (handle_echo(var));
	if (is_builtins(var->cmd_list[0]) == 6)
		handle_cd(var->cmd_list[1]);
	else if (is_builtins(var->cmd_list[0]) == 7)
		handle_env();
	else
		if (check_command2(var) == -1)
			return (-1);
	return (0);
}

int	parse(t_var *var)
{
	int		check;
	int		i;

	if (check_quotes(var->input))
		return (-1);
	var->cmd_list = split_input(var->input, ' ');
	i = 0;
	check = 0;
	while (!var->cmd_list[i])
	{
		var->cmd_list[i] = ft_strtrim(var->cmd_list[i], "'\"");
		i++;
	}
	check = check_command(var);
	free_list(var->cmd_list);
	return (check);
}
