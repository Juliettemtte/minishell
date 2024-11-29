/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:45:51 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/23 16:29:20 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	empty_env(t_var *var)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", pwd);
	var->og_envp = (char **)malloc(sizeof(char *) * 4);
	var->og_envp[0] = ft_strdup(pwd);
	var->og_envp[1] = ft_strdup("SHLVL=2");
	var->og_envp[2] = ft_strdup("_=/usr/bin/env");
	var->og_envp[3] = NULL;
}

void	copy_env(t_var *var, char **envp)
{
	int	i;
	int	envp_len;

	i = 0;
	envp_len = 0;
	while (envp[envp_len])
		envp_len++;
	if (envp_len == 0)
		empty_env(var);
	else
	{
		var->og_envp = (char **)malloc(sizeof(char *) * (envp_len + 1));
		if (!var->og_envp)
			return ;
		while (i < envp_len)
		{
			var->og_envp[i] = ft_strdup(envp[i]);
			i++;
		}
		var->og_envp[envp_len] = NULL;
	}
	var->envp = var->og_envp;
}

static int	check_args(t_token **token, int i)
{
	if (token[i + 1] && token[i + 1]->value[0] == '-'
		&& token[i + 1]->value[1] && ft_isascii(token[i + 1]->value[1]))
	{
		ft_putstr_fd("pwd: no option allowed\n", 2);
		return (2);
	}
	if (token[i + 1] && token[i + 1]->type == 2
		&& token[i + 1]->value[0] != '-')
	{
		ft_putstr_fd("pwd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	handle_env(t_var *var, t_token **token)
{
	int		i;
	int		j;
	char	*tmp;

	i = find_command_index(token, "env");
	j = check_args(token, i);
	if (j != 0)
		return (j);
	if (token[i + 1] && token[i + 1]->value[0] == '-')
		return (0);
	i = 0;
	while (var->envp[i] != NULL)
	{
		tmp = ft_strchr(var->envp[i], '=');
		if (tmp != 0 && tmp[1] != '\"' && tmp[2] != '\"')
			printf("%s\n", var->envp[i]);
		else if (tmp != 0)
		{
			tmp = ft_strtrim(var->envp[i], "\"");
			printf("%s\n", tmp);
			free(tmp);
		}
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
		printf("declare -x %s\n", sorted_env[j]);
		j++;
	}
	free(sorted_env);
	return (0);
}
