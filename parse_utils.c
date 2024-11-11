/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arissane <arissane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:31:07 by arissane          #+#    #+#             */
/*   Updated: 2024/11/11 14:20:41 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_env(t_var *var, int i)
{
	int		j;
	int		k;
	int		len;
	char	*temp;
	char	*envp;

	i++;
	j = i - 1;
	k = 0;
	len = 0;
	while (var->input[j++] && var->input[j] != ' '
		&& var->input[j] != '\"' && var->input[j] != '\'')
		len++;
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (1);
	while (k < len)
		temp[k++] = var->input[i++];
	temp[len] = '\0';
	envp = ft_getenv(temp, var->envp);
	free(temp);
	if (!envp)
		return (len + 1);
	return (0);
}

int	validate_heredoc_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] == ' ')
		i++;
	if (input[i] == '\0')
		return (1);
	while (input[i] && input[i] != ' ')
	{
		if (input[i] == '<' || input[i] == '>')
			return (1);
		i++;
	}
	return (0);
}

int	check_symbols(char c)
{
	if (!c)
		return (1);
	else if (c == '\"')
		return (1);
	else if (c == '\'')
		return (1);
	else if (c == ' ')
		return (1);
	else if (c == '?')
		return (1);
	else
		return (0);
}

void	copy1_with_space(char *input, char *str, int *i, int *k)
{
	str[(*k)++] = ' ';
	str[(*k)++] = input[(*i)++];
	str[(*k)++] = ' ';
}

void	copy2_with_space(char *input, char *str, int *i, int *k)
{
	str[(*k)++] = ' ';
	str[(*k)++] = input[(*i)++];
	str[(*k)++] = input[(*i)++];
	str[(*k)++] = ' ';
}
