/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:05:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/19 16:22:30 by jmouette         ###   ########.fr       */
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
		i++;
	}
	free(list);
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
			i++;
		}
		free(commands);
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

void	free_shell(t_var *var, t_token *tokens, t_token ***token_groups)
{
	if (var->input)
		free(var->input);
	free_token_groups(token_groups);
	free_tokens(tokens);
	free_env(&var->og_envp);
	if (var->cmd_list)
		free_list(var->cmd_list);
}
