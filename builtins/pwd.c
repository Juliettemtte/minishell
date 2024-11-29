/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:10:05 by arissane          #+#    #+#             */
/*   Updated: 2024/11/23 16:15:27 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_args(t_var *var, t_token **token)
{
	int	i;

	if (var->exit_code == 5)
		return (-1);
	i = find_command_index(token, "pwd");
	if (token[i + 1] && token[i + 1]->value && token[i + 1]->value[0] == '-'
		&& token[i + 1]->value[1] && ft_isascii(token[i + 1]->value[1]))
	{
		ft_putstr_fd("pwd: no option allowed\n", 2);
		return (2);
	}
	return (0);
}

static void	print_string_fd(char *str)
{
	ft_putstr_fd(str, 1);
	write(1, "\n", 1);
}

int	handle_pwd(t_var *var, t_token **token)
{
	char	*pwd_content;
	int		i;
	int		j;

	i = 0;
	j = check_args(var, token);
	if (j != 0)
		return (j);
	pwd_content = getcwd(NULL, 0);
	if (!pwd_content)
	{
		while (var->envp[i] != NULL)
		{
			if (ft_strncmp(var->envp[i], "PWD", 3) == 0)
			{
				print_string_fd(var->envp[i]);
				return (0);
			}
			i++;
		}
		return (1);
	}
	print_string_fd(pwd_content);
	free (pwd_content);
	return (0);
}
