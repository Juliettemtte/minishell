/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:58:17 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/09 17:44:54 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_command_index(t_token **tokens, const char *command)
{
	int	j;

	j = 0;
	while (tokens[j] != NULL && tokens[j]->value != NULL)
	{
		if (ft_strcmp(tokens[j]->value, command) == 0)
		{
			return (j);
		}
		j++;
	}
	return (-1);
}
