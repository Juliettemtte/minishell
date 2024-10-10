/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 18:16:12 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/09 13:30:17 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_identifier(const char *arg)
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

int	unset(char *name, size_t name_len)
{
	int	i;
	int	j;

	i = 0;
	while (environ[i] != NULL)
	{
		if (ft_strncmp(environ[i], name, name_len) == 0
			&& environ[i][name_len] == '=')
		{
			j = i;
			while (environ[j] != NULL)
			{
				environ[j] = environ[j + 1];
				j++;
			}
		}
		i++;
	}
	return (i);
}

int	handle_unset(t_token **token)
{
	int		i;

	i = find_command_index(token, "unset");
	if (i == -1)
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return (1);
	}
	while (token[i + 1] && token[i + 1]->type == 2)
	{
		if (is_valid_identifier(token[i + 1]->value))
			return (1);
		unset(token[i + 1]->value, ft_strlen(token[i + 1]->value));
		i++;
	}
	return (0);
}
