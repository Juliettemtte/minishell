/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:13:54 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/13 16:00:18 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i].value)
	{
		free(tokens[i].value);
		i++;
	}
	free(tokens);
}

//check if the next token after a redirection is valid
//(not a pipe, null, or a redirection)
static int	validate_redirections_pipes(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].value)
	{
		if (tokens[0].type == PIPE)
			return (1);
		if (tokens[i].type == PIPE && tokens[i + 1].type == PIPE)
			return (1);
		if (tokens[i].type > 4 && tokens[i + 1].value \
			&& tokens[i + 1].type > 3)
			return (1);
		if (tokens[i].type > 4 && !tokens[i + 1].value)
			return (1);
		i++;
	}
	return (0);
}

int	count_cmd_list(char **cmd_list)
{
	int	count;

	count = 0;
	if (cmd_list == NULL)
		return (0);
	while (cmd_list[count] != NULL)
		count++;
	return (count);
}

static void	create_token(char *cmd_part, t_token *tokens, t_var *var)
{
	char	*path;

	path = find_cmd_path(cmd_part, 0, var);
	if (!cmd_part)
		return ;
	else if (is_builtins(cmd_part) == 9)
		tokens->type = PIPE;
	else if (is_builtins(cmd_part) == 11)
		tokens->type = REDIRECTION_RIGHT;
	else if (is_builtins(cmd_part) == 13)
		tokens->type = APPEND;
	else if (is_builtins(cmd_part) == 12)
		tokens->type = REDIRECTION_LEFT;
	else if (is_builtins(cmd_part) == 10)
	{
		tokens->type = HEREDOC;
		tokens->heredoc_index = var->heredoc_count;
		var->heredoc_count++;
	}
	else if (is_builtins(cmd_part) != 0 || path != NULL )
		tokens->type = COMMAND;
	else
		tokens->type = ARGUMENT;
	free(path);
	tokens->value = ft_strdup(cmd_part);
}

int	tokenize_cmd_list(t_var *var, t_token *tokens)
{
	int	i;

	i = 0;
	var->heredoc_count = 0;
	if (!tokens || !var || !var->cmd_list)
		return (1);
	while (var->cmd_list[i])
	{
		create_token(var->cmd_list[i], &tokens[i], var);
		if (tokens[i].type == COMMAND)
			var->commands++;
		i++;
	}
	tokens[i].value = NULL;
	if (validate_redirections_pipes(tokens) == 1)
	{
		write(2, "syntax error\n", 13);
		return (2);
	}
	return (0);
}
