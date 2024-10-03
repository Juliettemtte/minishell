/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:45:51 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/03 16:00:09 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_env(void)
{
	int	i;

	i = 0;
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
}

void	print_env_sorted(void)
{
	int		i;
	int		j;
	char	**sorted_env;

	i = 0;
	j = 0;
	while (environ[i] != NULL)
		i++;
	sorted_env = malloc(i * sizeof(char *));
	if (sorted_env == NULL)
		return ;
	while (j < i)
	{
		sorted_env[j] = environ[j];
		j++;
	}
	bubble_sort(sorted_env, i);
	j = 0;
	while (j < i)
	{
		printf("%s\n", sorted_env[j]);
		j++;
	}
	free(sorted_env);
}

int	handle_unset(char *name, size_t name_len)
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

void	handle_export(char *name, char *value)
{
	int		i;
	int		j;
	char	*tmp;
	char	*new_var;
	char	**new_environ;

	value = ft_strtrim(value, "\"");
	tmp = ft_strjoin(name, "=");
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	free(value);
	i = handle_unset(name, ft_strlen(name));
	new_environ = malloc((i + 2) * sizeof(char *));
	if (new_environ == NULL)
	{
		free(new_var);
		return ;
	}
	j = -1;
	while (j++ < i)
		new_environ[j] = environ[j];
	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;
	environ = new_environ;
}
