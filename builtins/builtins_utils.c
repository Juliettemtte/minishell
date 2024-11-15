/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:58:17 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/15 12:59:06 by jmouette         ###   ########.fr       */
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

int	is_valid_identifier(const char *arg, char *cmd)
{
	int	i;

	i = 1;
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (1);
	}
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

void	change_env_cd(char *old_pwd, char *new_pwd, t_var *var)
{
	int	i;
	int	pwd_found;
	int	oldpwd_found;

	i = 0;
	pwd_found = 0;
	oldpwd_found = 0;
	while (var->envp[i] != NULL)
	{
		if (ft_strncmp(var->envp[i], "PWD", 3) == 0)
			pwd_found = 1;
		if (ft_strncmp(var->envp[i], "OLDPWD", 6) == 0)
			oldpwd_found = 1;
		i++;
	}
	if (oldpwd_found)
		export_cd("OLDPWD=", old_pwd, var);
	if (pwd_found)
		export_cd("PWD=", new_pwd, var);
	else if (oldpwd_found)
		export_cd("OLDPWD", "", var);
}
