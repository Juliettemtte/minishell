/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:05:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/02 16:00:50 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(char **list)
{
	int	i;

	i = 0;
	while (list[i])
	{
		free(list[i]);
		i++;
	}
	free(list);
}

void	free_command(char ***commands, int nb_cmd)
{
	int	i;

	i = 0;
	if (commands)
	{
		while (i < nb_cmd)
		{
			free_list(commands[i]);
			i++;
		}
		free(commands);
	}
}
