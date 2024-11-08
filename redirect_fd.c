/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:19:32 by arissane          #+#    #+#             */
/*   Updated: 2024/10/22 12:02:41 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_input(char *target, t_var *var)
{
	int		fd;

	fd = open(target, O_RDONLY, 0644);
	if (fd < 0)
	{
		perror("Can't open file for reading");
		return (1);
	}
	if (var->input_redir == 1)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 failed");
			close(fd);
			return (1);
		}
	}
	close(fd);
	return (0);
}

int	redirect_output(char *target, t_var *var)
{
	int		fd;

	fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Can't open file for writing");
		return (1);
	}
	if (var->output_redir == 1)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 failed");
			close(fd);
			return (1);
		}
	}
	close(fd);
	return (0);
}

int	redirect_append(char *target, t_var *var)
{
	int		fd;

	fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("Can't open file for appending");
		return (1);
	}
	if (var->output_redir == 1)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 failed");
			close(fd);
			return (1);
		}
	}
	close(fd);
	return (0);
}
