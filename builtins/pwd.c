/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:10:05 by arissane          #+#    #+#             */
/*   Updated: 2024/11/13 17:07:19 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pwd(t_var *var)
{
	char	*pwd_content;
	int		i;

	i = 0;
	if (var->exit_code == 5)
		return (-1);
	pwd_content = getcwd(NULL, 0);
	if (!pwd_content)
	{
		while (var->envp[i] != NULL)
		{
			if (ft_strncmp(var->envp[i], "PWD", 3) == 0)
			{
				ft_putstr_fd(var->envp[i], 1);
				write(1, "\n", 1);
				return (0);
			}
			i++;
		}
		return (1);
	}
	ft_putstr_fd(pwd_content, 1);
	write(1, "\n", 1);
	free (pwd_content);
	return (0);
}
