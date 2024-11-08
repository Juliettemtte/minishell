/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:29:33 by arissane          #+#    #+#             */
/*   Updated: 2024/10/30 11:43:36 by arissane         ###   ########.fr       */
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
	return (0);
}
