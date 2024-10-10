/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 18:16:49 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/09 17:41:36 by jmouette         ###   ########.fr       */
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

static char	*check_export(t_token **token, int i, char ***cmd)
{
	char	*equal_sign;
	char	*value;
	char	*temp;

	if (is_valid_identifier(token[i]->value))
		return (NULL);
	*cmd = malloc(2 * sizeof(char *));
	if (!(*cmd))
		return (NULL);
	equal_sign = ft_strchr(token[i]->value, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		(*cmd)[0] = ft_strdup(token[i]->value);
		(*cmd)[1] = ft_strdup(equal_sign + 1);
		if ((*cmd)[0] == NULL || (*cmd)[1] == NULL)
			return (NULL);
		value = ft_strjoin((*cmd)[0], "=");
		if (!value)
			return (NULL);
		temp = ft_strjoin(value, (*cmd)[1]);
		free(value);
		if (!temp)
			return (NULL);
		value = ft_strtrim(temp, "\"");
		free(temp);
		return (value);
	}
	else
	{
		(*cmd)[0] = ft_strdup(token[i]->value);
		(*cmd)[1] = ft_strdup("\'\'");
		value = ft_strjoin(*cmd[0], "=\'\'");
		return (value);
	}
	return (NULL);
}

int	handle_export(t_token **token_group)
{
	int		i;
	int		j;
	int		k;
	char	*new_var;
	char	**new_environ;
	char	**cmd;

	i = find_command_index(token_group, "export");
	if (i == -1)
		return (print_env_sorted());
	while (token_group[i + 1] && token_group[i + 1]->type == 2)
	{
		new_var = check_export(token_group, i + 1, &cmd);
		if (new_var == NULL)
			return (1);
		k = unset(cmd[0], ft_strlen(cmd[0]));
		new_environ = malloc((k + 2) * sizeof(char *));
		if (new_environ == NULL)
		{
			free(new_var);
			return (1);
		}
		j = -1;
		while (j++ < k)
			new_environ[j] = environ[j];
		new_environ[i] = new_var;
		new_environ[i + 1] = NULL;
		environ = new_environ;
		free(new_var);
		free_list(cmd);
		i++;
	}
	return (0);
}
