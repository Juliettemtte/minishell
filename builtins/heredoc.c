/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:10:36 by arissane          #+#    #+#             */
/*   Updated: 2024/10/08 10:58:11 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc(t_var *var)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (strcmp(line, var->cmd_list[1]) == 0)
			break ;
		var->heredoc = ft_strjoin(var->heredoc, line);
		var->heredoc = ft_strjoin(var->heredoc, "\n");
	}
	ft_putstr_fd(var->heredoc, 1);
	return (EXIT_SUCCESS);
}
