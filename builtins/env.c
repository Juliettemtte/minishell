/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:45:51 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/31 10:57:43 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_env(t_var *var, char **envp)
{
	int	i;
	int	envp_len;

	i = 0;
	envp_len = 0;
	while (envp[envp_len])
		envp_len++;
	var->og_envp = (char **)malloc(sizeof(char *) * (envp_len + 1));
	if (!var->og_envp)
		return ;
	while (i < envp_len)
	{
		var->og_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	var->og_envp[envp_len] = NULL;
	var->envp = var->og_envp;
}

int	handle_env(t_var *var)
{
	int	i;

	i = 0;
	ft_unset("COLUMNS", ft_strlen("COLUMNS"), var);
	ft_unset("LINES", ft_strlen("LINES"), var);
	while (var->envp[i] != NULL)
	{
		printf("%s\n", var->envp[i]);
		i++;
	}
	return (0);
}

int	print_env_sorted(t_var *var)
{
	int		i;
	int		j;
	char	**sorted_env;

	i = 0;
	j = 0;
	while (var->envp[i] != NULL)
		i++;
	sorted_env = malloc(i * sizeof(char *));
	if (sorted_env == NULL)
		return (1);
	while (j < i)
	{
		sorted_env[j] = var->envp[j];
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
	return (0);
}
