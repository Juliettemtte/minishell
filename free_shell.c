/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:05:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/14 13:35:08 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(char **list)
{
	int	i;

	i = 0;
	if (!list)
		return ;
	while (list[i])
	{
		free(list[i]);
		list[i] = NULL;
		i++;
	}
	free(list);
	list = NULL;
}

void	free_command(char ***commands)
{
	int	i;

	i = 0;
	if (commands)
	{
		while (commands[i])
		{
			free_list(commands[i]);
			commands[i] = NULL;
			i++;
		}
		free(commands);
		commands = NULL;
	}
}

void	free_token_groups(t_token ***token_groups)
{
	int	i;
	int	k;

	i = 0;
	if (token_groups)
	{
		while (token_groups[i])
		{
			k = 0;
			while (token_groups[i][k])
			{
				free(token_groups[i][k]->value);
				free(token_groups[i][k]);
				k++;
			}
			free(token_groups[i]);
			i++;
		}
		free(token_groups);
	}
}

void	free_env(char ***env)
{
	int	i;

	i = 0;
	if (env && *env)
	{
		while ((*env)[i] != NULL)
		{
			free((*env)[i]);
			(*env)[i] = NULL;
			i++;
		}
		if (*env)
			free(*env);
		*env = NULL;
	}
}

void	free_shell(t_var *var)
{
	if (var->input)
		free(var->input);
	if (var->token_groups)
		free_token_groups(var->token_groups);
	if (var->tokens)
		free_tokens(var->tokens);
	if (var->cmd_list)
		free_list(var->cmd_list);
}
