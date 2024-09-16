/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:11:25 by arissane          #+#    #+#             */
/*   Updated: 2024/09/09 11:39:03 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//open file found at *target, or create it if it doesn't exist, and replace the old content with *content
void	redirectreplace_output_right(char *content, char *target)
{
	int	target_fd;

	if (!target)
		return ;
	target_fd = open(target, O_RDWR | O_CREAT | O_TRUNC, 0644);
	ft_putstr_fd(content, target_fd);
	close(target_fd);
}

//do the same, except don't replace the old content, add at the end of it
void	redirect_output_right(char *content, char *target)
{
	int	target_fd;

	if (!target)
		return ;
	target_fd = open(target, O_RDWR | O_CREAT | O_APPEND, 0644);
	ft_putstr_fd(content, target_fd);
	close(target_fd);
}
