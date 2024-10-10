/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:45:51 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/09 16:23:51 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_env(void)
{
	int	i;

	i = 0;
	unset("COLUMNS", ft_strlen("COLUMNS"));
	unset("LINES", ft_strlen("LINES"));
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

int	print_env_sorted(void)
{
	int		i;
	int		j;
	char	**sorted_env;

	i = 0;
	j = 0;
	while (environ[i] != NULL)
		i++;
	sorted_env = malloc(i * sizeof(char *));
	if (sorted_env == NULL)
		return (1);
	while (j < i)
	{
		sorted_env[j] = environ[j];
		j++;
	}
	bubble_sort(sorted_env, i);
	j = 0;
	while (j < i)
	{
		printf("%s\n", sorted_env[j]);
		j++;
	}
	free(sorted_env);
	return (0);
}
