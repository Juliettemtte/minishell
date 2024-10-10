/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:10:05 by arissane          #+#    #+#             */
/*   Updated: 2024/10/09 09:37:31 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pwd(t_var *var)
{
	char	*pwd_content;

	if (var->exit_code == 5)
		return (-1);
	pwd_content = getcwd(NULL, 0);
	if (!pwd_content)
		exit(EXIT_FAILURE);
	ft_putstr_fd(pwd_content, 1);
	write(1, "\n", 1);
	free (pwd_content);
	return (0);
}
