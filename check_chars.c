/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:44:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/16 16:35:03 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_new_string(char *str, int i, char *value, int end)
{
	size_t	len;
	char	*temp;
	int		k;
	int		j;

	len = ft_strlen(str) - (end - i) + ft_strlen(value);
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (NULL);
	k = 0;
	while (k < i)
	{
		temp[k] = str[k];
		k++;
	}
	j = 0;
	while (value[j])
		temp[k++] = value[j++];
	while (str[end])
		temp[k++] = str[end++];
	temp[k] = '\0';
	free(str);
	return (temp);
}

char	*get_env_value(t_var *var, char *str, int i)
{
	char	*value;
	char	*temp;
	int		end;

	end = i + 1;
	if (str[i + 1] == '?')
	{
		value = ft_itoa(var->exit_code);
		end = i + 2;
	}
	else
	{
		end = i + 1;
		while (str[end] && str[end] != ' '
			&& str[end] != '\"' && str[end] != '\'')
			end++;
		temp = ft_substr(str, i + 1, end - (i + 1));
		value = getenv(temp);
		free(temp);
	}
	return (create_new_string(str, i, value, end));
}

static char	*remove_quotes(t_var *var, char *str)
{
	int		i;
	int		j;
	char	c;
	int		depth;
	int		outer_quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			while (str[i + 1] != '\'')
				i++;
			i += 2;
		}
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' '
			&& str[i + 1] != '\"')
		{
			str = get_env_value(var, str, i);
		}
		i++;
	}
	i = 0;
	j = 0;
	depth = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			c = str[i];
			if (depth == 0)
			{
				outer_quote = c;
				depth++;
				i++;
				continue ;
			}
			else if (str[i] == outer_quote && depth == 1)
			{
				depth--;
				i++;
				continue ;
			}
		}
		str[j++] = str[i++];
	}
	while (str[j])
	{
		str[j] = '\0';
		j++;
	}
	return (str);
}

void	check_characters(t_var *var, t_token **token_group)
{
	int	i;

	i = 0;
	while (token_group[i])
	{
		token_group[i]->value = remove_quotes(var, token_group[i]->value);
		i++;
	}
}
