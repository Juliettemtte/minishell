/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:22:51 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/09 17:40:05 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	bubble_sort(char **arr, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
				ft_swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

static int	is_builtins2(char *cmd)
{
	if (ft_strcmp(cmd, "|") == 0)
		return (9);
	if (ft_strcmp(cmd, "<<") == 0)
		return (10);
	if (ft_strcmp(cmd, ">") == 0)
		return (11);
	if (ft_strcmp(cmd, "<") == 0)
		return (12);
	if (ft_strcmp(cmd, ">>") == 0)
		return (13);
	return (0);
}

int	is_builtins(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "EOF") == 0)
		return (2);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (3);
	if (ft_strcmp(cmd, "echo") == 0)
		return (4);
	if (ft_strcmp(cmd, "cd") == 0)
		return (5);
	if (ft_strcmp(cmd, "env") == 0)
		return (6);
	if (ft_strcmp(cmd, "export") == 0)
		return (7);
	if (ft_strcmp(cmd, "unset") == 0)
		return (8);
	return (is_builtins2(cmd));
}
