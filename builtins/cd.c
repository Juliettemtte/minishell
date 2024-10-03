/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:25:42 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/03 17:09:03 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_cd(t_token **token, int i, char **new_path)
{
	if (!token[i + 1] || is_builtins(token[i + 1]->value) != 0)
	{
		*new_path = getenv("HOME");
		if (!(*new_path))
			return (1);
		return (2);
	}
	if (access(token[i + 1]->value, F_OK) != 0)
	{
		printf("cd: %s: No such file or directory\n", token[i + 1]->value);
		return (1);
	}
	if (token[i + 2] && token[i + 2]->type == 2)
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (token[i + 1]->value[0] == '$')
	{
		*new_path = getenv(token[i + 1]->value + 1);
		if (!(*new_path))
			return (1);
		return (2);
	}
	return (0);
}

int	handle_cd(t_token **token_group)
{
	char	new_cwd[260]; //260 = MAX_PATH
	char	old_path[260];
	char	*new_path;
	int		i;
	int		result;

	if (!getcwd(old_path, sizeof(old_path)))
		exit(EXIT_FAILURE);
	i = 0;
	while (token_group[i] && token_group[i]->value)
	{
		if (is_builtins(token_group[i]->value) == 6)
			break ;
		i++;
	}
	result = check_cd(token_group, i, &new_path);
	if (result == 1)
		exit(EXIT_FAILURE);
	else if (result == 0)
		new_path = token_group[i + 1]->value;
	if (new_path && chdir(new_path) == 0)
	{
		if (!getcwd(new_cwd, sizeof(new_cwd)))
			exit(EXIT_FAILURE);
		handle_export("OLDPWD", old_path);
		handle_export("PWD", new_cwd);
	}
	else
	{
		printf("cd: %s: No such file or directory\n", new_path);
		exit(EXIT_FAILURE);
	}
	return (0);
}
