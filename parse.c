/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/10/18 18:06:32 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_env(char *input, int i)
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
	while (input[j++] && input[j] != ' '
		&& input[j] != '\"' && input[j] != '\'')
		len++;
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (0);
	while (k < len)
		temp[k++] = input[i++];
	temp[len] = '\0';
	envp = getenv(temp);
	free(temp);
	if (!envp)
		return (len + 1);
	return (0);
}

int	validate_heredoc_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != ' ')
	{
		if (input[i] == '<' || input[i] == '>')
			return (1);
		i++;
	}
	return (0);
}

char	*split_redirections(char *input)
{
	int		i;
	int		k;
	int		len;
	char	*str;

	i = 0;
	k = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			k += 2;
		i++;
		k++;
	}
	str = malloc(sizeof(char) * (k + 1));
	if (!str)
		return (NULL);
	i = 0;
	k = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && input[i + 1] != '\"'
			&& input[i + 1] != '\'' && input[i + 1] != ' '
			&& input[i + 1] != '?')
		{
			len = check_env(input, i);
			if (len > 0)
				i += len;
			else
				str[k++] = input[i++];
		}
		else if (input[i] == '\"' && input[i + 1] == '\"')
			i += 2;
		else if (input[i] == '\'' && input[i + 1] == '\'')
			i += 2;
		else if (input[i] == '\"')
		{
			str[k++] = input[i++];
			while (input[i] && input[i] != '\"')
			{
				if (input[i] == '$' && input[i + 1] && input[i + 1] != '?'
					&& input[i + 1] != ' ' && input[i + 1] != '\"'
					&& input[i + 1] != '\'')
				{
					len = (check_env(input, i));
					if (len > 0)
						i += len;
					else
						str[k++] = input[i++];
				}
				else
					str[k++] = input[i++];
			}
			if (input[i] == '\"')
			{
				str[k++] = input[i++];
			}
			else
			{
				printf("unclosed quotes\n");
				free(str);
				return (NULL);
			}
		}
		else if (input[i] == '\'')
		{
			str[k++] = input[i++];
			while (input[i] && input[i] != '\'')
			{
				str[k++] = input[i++];
			}
			if (input[i] == '\'')
			{
				str[k++] = input[i++];
			}
			else
			{
				printf("unclosed quotes\n");
				free(str);
				return (NULL);
			}
		}
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			if (validate_heredoc_input(&input[i + 2]) == 1)
			{
				printf("heredoc syntax\n");
				free (str);
				return (NULL);
			}
			str[k++] = ' ';
			str[k++] = input[i++];
			str[k++] = input[i++];
			str[k++] = ' ';
		}
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			str[k++] = ' ';
			str[k++] = input[i++];
			str[k++] = input[i++];
			str[k++] = ' ';
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			str[k++] = ' ';
			str[k++] = input[i++];
			str[k++] = ' ';
		}
		else if (input[i] == '|')
		{
			str[k++] = ' ';
			str[k++] = input[i++];
			str[k++] = ' ';
		}
		else
			str[k++] = input[i++];
	}
	str[k] = '\0';
	return (str);
}

int	parse(t_var *var)
{
	int		i;
	char	*input;

	input = split_redirections(var->input);
	if (!input)
	{
		printf("parse error\n");
		return (-1);
	}
	var->cmd_list = split_input(input, ' ');
	free(input);
	if (!var->cmd_list)
		return (1);
	i = 0;
	while (var->cmd_list[i])
	{
		if (!var->cmd_list[i])
			return (1);
		if (is_builtins(var->cmd_list[i]) >= 9)
			var->is_redirect = 1;
		if (is_builtins(var->cmd_list[i]) == 9)
		{
			if ((var->cmd_list[i + 1] && var->cmd_list[i + 1][0] == '|')
				|| !var->cmd_list[i + 1])
				return (-1);
			var->pipes++;
		}
		if (is_builtins(var->cmd_list[i]) == 10)
			var->heredoc_count++;
		i++;
	}
	//dynamically allocate space for each heredoc fd
	if (var->heredoc_count > 0)
	{
		var->heredoc_fds = malloc(sizeof(int) * (var->heredoc_count + 1));
		if (!var->heredoc_fds)
			return (-1);
		i = 0;
		while (i < var->heredoc_count)
		{
			var->heredoc_fds[i] = -1;
			i++;
		}
		var->heredoc_fds[i] = -1;
		var->heredoc_count = 0;
	}
	return (0);
}
