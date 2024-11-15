/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:38:02 by arissane          #+#    #+#             */
/*   Updated: 2024/11/15 12:34:53 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_double_quotes(t_var *var, int *k, int i)
{
	int	len;

	var->str[(*k)++] = var->input[i++];
	while (var->input[i] && var->input[i] != '\"')
	{
		if (var->input[i] == '$' && check_symbols(var->input[i + 1]) == 0)
		{
			len = (check_env(var, i + 1, 0));
			if (len > 0)
				i += len;
			else
				var->str[(*k)++] = var->input[i++];
		}
		else
			var->str[(*k)++] = var->input[i++];
	}
	if (var->input[i] == '\"')
		var->str[(*k)++] = var->input[i++];
	else
	{
		return (-1);
	}
	return (i);
}

static int	process_special_characters3(t_var *var, int *i, int *k)
{
	if (var->input[*i] == '<' && var->input[*i + 1] == '<')
	{
		if (validate_heredoc_input(&var->input[*i + 2]) == 1)
			return (2);
		copy2_with_space(var->input, var->str, i, k);
	}
	else if (var->input[*i] == '>' && var->input[*i + 1] == '>')
		copy2_with_space(var->input, var->str, i, k);
	else if (var->input[*i] == '<' || var->input[*i] == '>')
		copy1_with_space(var->input, var->str, i, k);
	else if (var->input[*i] == '|')
		copy1_with_space(var->input, var->str, i, k);
	else
		var->str[(*k)++] = var->input[(*i)++];
	return (0);
}

static int	process_special_characters2(t_var *var, int *i, int *k)
{
	if (var->input[*i] == '\"')
	{
		*i = (process_double_quotes(var, k, *i));
		if (*i == -1)
			return (1);
	}
	else if (var->input[*i] == '\'')
	{
		var->str[(*k)++] = var->input[(*i)++];
		while (var->input[*i] && var->input[*i] != '\'')
			var->str[(*k)++] = var->input[(*i)++];
		if (var->input[*i] == '\'')
			var->str[(*k)++] = var->input[(*i)++];
		else
			return (1);
	}
	else if (process_special_characters3(var, i, k) == 2)
		return (2);
	return (0);
}

static int	process_special_characters(t_var *var, int check, int i, int k)
{
	int	len;

	while (var->input[i])
	{
		if (var->input[i] == '$' && check_symbols(var->input[i + 1]) == 0)
		{
			len = check_env(var, i + 1, 0);
			if (len > 0)
				i += len;
			else
				var->str[k++] = var->input[i++];
		}
		else if (var->input[i] == '\t')
		{
			var->str[k++] = ' ';
			i++;
		}
		else
			check = process_special_characters2(var, &i, &k);
		if (check == 1 || check == 2)
			return (check);
	}
	var->str[k] = '\0';
	return (0);
}

int	parse_add_spaces(t_var *var, int i, int k)
{
	int		check;

	while (var->input[i])
	{
		if (var->input[i] == '>' || var->input[i] == '<'
			|| var->input[i] == '|')
			k += 2;
		i++;
		k++;
	}
	var->str = malloc(sizeof(char) * (k + 1));
	if (!var->str)
		return (1);
	check = process_special_characters(var, 0, 0, 0);
	if (check == 1 || check == 2)
	{
		if (check == 1)
			ft_putstr_fd("unclosed quotes\n", 2);
		if (check == 2)
			ft_putstr_fd("invalid heredoc delimiter\n", 2);
		free(var->str);
		return (1);
	}
	return (0);
}
