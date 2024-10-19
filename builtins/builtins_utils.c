/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:58:17 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/15 15:20:38 by jmouette         ###   ########.fr       */
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

int	is_valid_identifier(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		return (1);
	}
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}
