/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 18:16:49 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/07 14:07:44 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_equal_sign(t_token **token, char *str, char ***cmd, int i)
{
	char	*temp;
	char	*value;

	*str = '\0';
	(*cmd)[0] = ft_strdup(token[i]->value);
	(*cmd)[1] = ft_strdup(str + 1);
	if ((*cmd)[0] == NULL || (*cmd)[1] == NULL)
		return (NULL);
	value = ft_strjoin((*cmd)[0], "=");
	if (!value)
		return (NULL);
	temp = ft_strjoin(value, (*cmd)[1]);
	free(value);
	if (!temp)
		return (NULL);
	return (temp);
}

static char	*check_export(t_token **token, int i, char ***cmd)
{
	char	*equal_sign;
	char	*value;

	if (is_valid_identifier(token[i]->value))
		return (NULL);
	*cmd = malloc(2 * sizeof(char *));
	if (!(*cmd))
		return (NULL);
	equal_sign = ft_strchr(token[i]->value, '=');
	if (equal_sign)
		return (handle_equal_sign(token, equal_sign, cmd, i));
	else
	{
		(*cmd)[0] = ft_strdup(token[i]->value);
		(*cmd)[1] = ft_strdup("\'\'");
		value = ft_strjoin(*cmd[0], "=\'\'");
		return (value);
	}
	return (NULL);
}

static int	set_environment_variable(char *name, const char *value, t_var *var)
{
	int		k;
	int		j;
	char	**new_environ;

	k = ft_unset(name, ft_strlen(name), var);
	new_environ = malloc((k + 2) * sizeof(char *));
	if (new_environ == NULL)
		return (0);
	j = 0;
	while (j <= k)
	{
		new_environ[j] = var->envp[j];
		j++;
	}
	new_environ[k] = ft_strdup(value);
	new_environ[k + 1] = NULL;
	free(var->envp);
	var->envp = new_environ;
	return (1);
}

static int	export_variable(t_token **token_group, int index, t_var *var)
{
	char	*new_var;
	char	**cmd;
	int		result;

	new_var = check_export(token_group, index, &cmd);
	result = 1;
	if (new_var == NULL)
		return (0);
	if (!set_environment_variable(cmd[0], new_var, var))
		result = 0;
	free(cmd[0]);
	free(cmd[1]);
	free(cmd);
	free(new_var);
	return (1);
}

int	handle_export(t_token **token_group, t_var *var)
{
	int		i;

	i = find_command_index(token_group, "export");
	if (i == 0 && token_group[i + 1] == NULL)
		return (print_env_sorted(var));
	while (token_group[i + 1] && token_group[i + 1]->type == 2)
	{
		if (!export_variable(token_group, i + 1, var))
			return (1);
		i++;
	}
	return (0);
}
