/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:43:11 by arissane          #+#    #+#             */
/*   Updated: 2024/11/19 13:41:46 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init(int *std_in, int *std_out, t_token **tokens, t_var *var)
{
	int	i;
	int	count;

	if (ft_strcmp(tokens[0]->value, "\"\"") != 0)
		check_characters(tokens);
	*std_in = dup(STDIN_FILENO);
	*std_out = dup(STDOUT_FILENO);
	if (*std_in == -1 || *std_out == -1)
	{
		perror("dup failed");
		return (-1);
	}
	i = 0;
	count = 0;
	while (tokens[i])
	{
		if (tokens[i]->type > 4)
			count++;
		if (tokens[i]->type == 5 || tokens[i]->type == 8)
			var->input_redir++;
		if (tokens[i]->type == 6 || tokens[i]->type == 7)
			var->output_redir++;
		i++;
	}
	return (count);
}

static int	restore_io(int std_in, int std_out, int return_code)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close(std_in);
	close(std_out);
	return (return_code);
}

static int	process_redirection(t_token *token, t_token *next_token, t_var *var)
{
	int		check;

	check = 0;
	if (token->type == REDIRECTION_LEFT)
		check = redirect_input(next_token->value, var);
	else if (token->type == REDIRECTION_RIGHT)
		check = redirect_output(next_token->value, var);
	else if (token->type == APPEND)
		check = redirect_append(next_token->value, var);
	else if (token->type == HEREDOC)
		check = redirect_heredoc(var, token);
	return (check);
}

int	handle_redirect(t_var *var, t_token **tokens, int check)
{
	int	i;
	int	std_in;
	int	std_out;

	if (init(&std_in, &std_out, tokens, var) == -1)
		return (-1);
	if (var->input_redir == 0 && var->output_redir == 0)
		return (restore_io(std_in, std_out, -2));
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type > 4 && tokens[i]->type < 9)
		{
			check = process_redirection(tokens[i], tokens[i + 1], var);
			if (check != 0)
				return (restore_io(std_in, std_out, check));
			if (tokens[i]->type == 5 || tokens[i]->type == 8)
				var->input_redir--;
			if (tokens[i]->type == 6 || tokens[i]->type == 7)
				var->output_redir--;
		}
		i++;
	}
	check = run_command(var, tokens);
	return (restore_io(std_in, std_out, check));
}
