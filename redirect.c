/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:11:25 by arissane          #+#    #+#             */
/*   Updated: 2024/09/18 11:51:51 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//open file found at *target, or create it if it doesn't exist, and replace the old content with *content
int	redirectreplace_output_right(char *target)
{
	int	target_fd;

	if (!target)
		return (-1);
	target_fd = open(target, O_RDWR | O_CREAT | O_TRUNC, 0644);
//	ft_putstr_fd(content, target_fd);
//	close(target_fd);
	return (target_fd);
}

//do the same, except don't replace the old content, add at the end of it
int	redirect_output_right(char *target)
{
	int	target_fd;

	if (!target)
		return (-1);
	target_fd = open(target, O_RDWR | O_CREAT | O_APPEND, 0644);
//	ft_putstr_fd(content, target_fd);
//	close(target_fd);
	return (target_fd);
}

int	check_redirect(char **cmd_list)
{
	if (cmd_list[1] && cmd_list[2] && cmd_list[3])
	{
		if (ft_strncmp(cmd_list[2], ">", 2) == 0)
			return (redirectreplace_output_right(cmd_list[3]));
		else if (ft_strncmp(cmd_list[2], ">>", 3) == 0)
			return (redirect_output_right(cmd_list[3]));
	}
	return (1);
}
