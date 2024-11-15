/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:29:33 by arissane          #+#    #+#             */
/*   Updated: 2024/11/14 15:01:21 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_envcmp(char *envp, char *str)
{
	size_t	i;

	i = 0;
	while (envp[i] != '=')
	{
		if (str[i] != envp[i])
			return (1);
		i++;
	}
	if (str[i] != '\0')
		return (1);
	return (0);
}

long	ft_atol(const char *str, int i)
{
	long		sign;
	long long	result;

	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			if (sign == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

int	validate_cmd_path(char *cmd)
{
	int	error_code;

	error_code = 0;
	if (access(cmd, F_OK) != 0)
	{
		error_code = 127;
		ft_putstr_fd(" No such file or directory\n", 2);
	}
	else if (opendir(cmd) != NULL)
	{
		error_code = 126;
		ft_putstr_fd(" Is a directory\n", 2);
	}
	else if (access(cmd, X_OK) != 0)
	{	
		error_code = 126;
		ft_putstr_fd(" Permission denied\n", 2);
	}
	return (error_code);
}
