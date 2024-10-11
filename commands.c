/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:55:28 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/11 16:23:39 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_var *var, char *str, int i)
{
	char	*value;
	char	*temp;
	int		j;
	int		k;
	int		end;
	size_t	len;

	temp = NULL;
	if (str[i + 1] == '?')
	{
		value = ft_itoa(var->exit_code);
		end = i + 2;
	}
	else
	{
		end = i + 1;
		while (str[end] && str[end] != ' ' &&
				str[end] != '\"' && str[end] != '\'')
			end++;
		temp = ft_substr(str, i + 1, end - (i + 1));
		value = getenv(temp);
		free(temp);
	}
	if (!value)
		value = "";
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
	k = 0;
	j = i;
	while (value[k])
	{
		temp[j] = value[k];
		j++;
		k++;
	}
	while (str[end])
		temp[j++] = str[end++];
	temp[j] = '\0';
	free(str);
	return (temp);
}

char	*remove_quotes(t_var *var, char *str)
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
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' &&
				str[i + 1] != '\"')
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

int	run_command(t_var *var, t_token **token_group)
{
	char	*cmd;

	if (!token_group)//this happens if you only press enter
		return (0);
	check_characters(var, token_group);
	cmd = token_group[0]->value;
	if (is_builtins(cmd) == 1)
		return (-2);
	if (is_builtins(cmd) == 2)
		return (0);
	if (is_builtins(cmd) == 9)
		return (handle_heredoc(var));
	if (is_builtins(cmd) == 3)
		return (handle_pwd(var));
	if (is_builtins(cmd) == 4)
		return (handle_echo(token_group));
	if (is_builtins(cmd) == 5)
		return (handle_cd(token_group));
	if (is_builtins(cmd) == 6)
		return (handle_env());
	if (is_builtins(cmd) == 7)
		return(handle_export(token_group));
	if (is_builtins(cmd) == 8)
		return (handle_unset(token_group));
	return (execute_command(token_group));
}
