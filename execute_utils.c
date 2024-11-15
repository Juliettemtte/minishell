/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:02:48 by arissane          #+#    #+#             */
/*   Updated: 2024/11/15 13:06:00 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exec_errors(char *command)
{
	int	error_code;

	error_code = 0;
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
		error_code = validate_cmd_path(command);
	else
	{
		error_code = 127;
		ft_putstr_fd(" command not found\n", 2);
	}
	return (error_code);
}

char	**fill_args(t_token **token, char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (token[j])
	{
		if (token[j]->type > 4 && token[j]->type < 9)
			j++;
		else if (token[j]->type == 2 || token[j]->type == 1)
		{
			args[i] = token[j]->value;
			i++;
		}
		j++;
	}
	args[i] = NULL;
	return (args);
}
