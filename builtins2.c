/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:32:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/20 10:32:20 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_var *var)
{
	char	*line;
	int	fd;

	fd = check_redirect(var->cmd_list);
	while (1)
	{
		line = readline("> ");
		if (strcmp(line, var->cmd_list[1]) == 0)
			break ;
		var->heredoc = ft_strjoin(var->heredoc, line);
		var->heredoc = ft_strjoin(var->heredoc, "\n");
	}
	ft_putstr_fd(var->heredoc, fd);
	if (fd > 1)
		close (fd);
	return (0);
}

int	handle_pwd(t_var *var)
{
	char	*pwd_content;
	int	fd;

	pwd_content = getcwd(NULL, 0);
	fd = check_redirect(var->cmd_list);
	ft_putstr_fd(pwd_content, fd);
	write(fd, "\n", 1);
	if (fd > 1)
		close (fd);
	return (0);
}

//skip over the valid redirections and heredocs like the real echo while looking for the input
static int	check_exceptions(char **cmd_list, int i)
{
	if (cmd_list[i] && cmd_list[i + 1])
	{
		if (ft_strcmp(cmd_list[i], ">") == 0)
			return (0);
		if (ft_strcmp(cmd_list[i], ">>") == 0)
			return (0);
		if (ft_strcmp(cmd_list[i], "<<") == 0)
			return (0);
	}
	return (-1);
}

int	handle_echo(char **cmd_list)
{
	int	i;
	int	check_n;
	int	fd;

	i = 1;
	check_n = 0;
	fd = check_redirect(cmd_list);
	if (cmd_list[i] && ft_strcmp(cmd_list[i], "-n") == 0)
		check_n = 1;
	if (check_n == 1)
		i++;
	while (cmd_list[i])
	{
		while (check_exceptions(cmd_list, i) == 0)
			i += 2;
		if (cmd_list[i])
		{
			ft_putstr_fd(cmd_list[i], fd);
			if (cmd_list[i + 1] && cmd_list[i][0] != '\0')
				write(fd, " ", 1);
			i++;
		}
	}
	if (check_n == 0)
		write(fd, "\n", 1);
	if (fd > 1)
		close(fd);
	return (0);
}
