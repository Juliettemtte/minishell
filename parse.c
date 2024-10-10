/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/10/10 13:03:20 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*split_redirections(char *input)
{
	int	i;
//	int	j;
	int	k;
	char	*str;
//	char	*nb_str;

	i = 0;
	k = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
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
		if (input[i] == '\"' && input[i + 1] == '\"')
			i += 2;
		else if (input[i] == '\'' && input[i + 1] == '\'')
			i += 2;
		else if (input[i] == '\"')
		{
			str[k++] = input[i++];
			while (input[i] && input[i] != '\"')
			{
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
		else if ((input[i] == '<' && input[i + 1] == '<') ||
				(input[i] == '>' && input[i + 1] == '>'))
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
		/*else if (input[i] == '$' && input[i + 1] == '?')
		{
			j = 0;
			nb_str = ft_itoa(var->exit_code);
			while (nb_str[j])
				str[k++] = nb_str[j++];
			free(nb_str);
			i += 2;
		}*/
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
		return (1);
	}
	var->cmd_list = split_input(input, ' ');
	free(input);
	if (!var->cmd_list)
		return (1);
	i = 0;
	while (var->cmd_list[i])
	{
//		printf("%s\n", var->cmd_list[i]);
//		var->cmd_list[i] = ft_strtrim(var->cmd_list[i], "\'\"");
		if (!var->cmd_list[i])
			return (1);
		if (is_builtins(var->cmd_list[i]) >= 9)
			var->is_redirect = 1;
		if (is_builtins(var->cmd_list[i]) == 9) //check the number of pipes. If there is no valid argument after the pipe, return
		{
			if ((var->cmd_list[i + 1] && var->cmd_list[i + 1][0] == '|') || !var->cmd_list[i + 1])
				return (-1);
			var->pipes++;
		}
		i++;
	}
	return (0);
}
