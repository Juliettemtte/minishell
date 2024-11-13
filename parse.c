/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 09:53:07 by arissane          #+#    #+#             */
/*   Updated: 2024/11/13 16:01:56 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_cmd_list(t_var *var)
{
	int	i;

	i = 0;
	while (var->cmd_list[i])
	{
		if (is_builtins(var->cmd_list[i]) >= 9)
			var->is_redirect = 1;
		if (is_builtins(var->cmd_list[i]) == 9)
		{
			if ((var->cmd_list[i + 1] && var->cmd_list[i + 1][0] == '|')
				|| !var->cmd_list[i + 1])
				return (2);
			var->pipes++;
		}
		if (is_builtins(var->cmd_list[i]) == 10)
			var->heredoc_count++;
		i++;
	}
	return (0);
}

static char	*expand_global_variables(t_var *var, char *str)
{
	int		depth;
	int		i;
	int		heredoc;
	char	outer_quote;

	i = 0;
	depth = 0;
	heredoc = 0;
	while (str[i])
	{
		heredoc = 0;
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (is_inside_quotes(str[i], &depth, &outer_quote) == 0)
			{
				i++;
				continue ;
			}
		}
		if (i > 0 && str[i - 1] == '<' && str[i] == '<' && depth == 0)
		{
			i++;
			while (str[i] && (str[i] == ' ' || str[i] == '\t'))
				i++;
			while (str[i] && str[i] != ' ')
				i++;
			heredoc = 1;
		}
		if (outer_quote != '\'' && str[i] == '$' && str[i + 1] \
			&& str[i + 1] != ' ' && str[i + 1] != '\"')
		{
			str = get_env_value(var, str, i);
		}
		if (heredoc == 0)
			i++;
	}
	return (str);
}

int	parse(t_var *var)
{
	if (split_redirections(var) == 1)
		return (2);
	var->str = expand_global_variables(var, var->str);
	var->cmd_list = split_input(var->str, ' ');
	free(var->str);
	if (!var->cmd_list)
		return (1);
	if (!var->cmd_list[0])
	{
		var->status = EMPTY_LINE;
		return (3);
	}
	return (parse_cmd_list(var));
}
