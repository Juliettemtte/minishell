/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:43:11 by arissane          #+#    #+#             */
/*   Updated: 2024/10/18 18:07:30 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init(int *std_in, int *std_out, t_token **tokens, t_var *var)
{
	int	i;
	int	count;

	check_characters(var, tokens);
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

static int	redirect_input(char *arg, int mode, char *error, int fd_target, t_var *var)
{
	int		fd;

	fd = open(arg, mode, 0644);
	if (fd < 0)
	{
		perror(error);
		return (-1);
	}
	if (var->output_redir == 1 || var->input_redir == 1)
	{
		if (dup2(fd, fd_target) < 0)
		{
			perror("dup2 failed");
			close(fd);
			return (-1);
		}
	}
	close(fd);
	return (1);
}

static int	process_redirection(t_token *token, t_token *next_token, t_var *var)
{
	int		check;
	char	*error_message;

	check = 0;
	if (token->type == REDIRECTION_LEFT)
	{	
		error_message = "Can't open file for reading";
		check = redirect_input(next_token->value,
				O_RDONLY, error_message, STDIN_FILENO, var);
	}
	else if (token->type == REDIRECTION_RIGHT)
	{	
		error_message = "Can't open file for writing";
		check = redirect_input(next_token->value,
				O_WRONLY | O_CREAT | O_TRUNC, error_message, STDOUT_FILENO, var);
	}
	else if (token->type == APPEND)
	{	
		error_message = "Can't open file for appending";
		check = redirect_input(next_token->value,
				O_WRONLY | O_CREAT | O_APPEND, error_message, STDOUT_FILENO, var);
	}
	else if (token->type == HEREDOC)
		check = redirect_heredoc(var, token);
	return (check);
}

int	handle_redirect(t_var *var, t_token **tokens)
{
	int	i;
	int	std_in;
	int	std_out;
	int	check;

	if (init(&std_in, &std_out, tokens, var) == -1)
		return (-1);
	if (var->input_redir == 0 && var->output_redir == 0)
		return (restore_io(std_in, std_out, 3));
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type > 4 && tokens[i]->type < 9)
		{
			check = process_redirection(tokens[i], tokens[i + 1], var);
			if (check == -1)
				return (restore_io(std_in, std_out, 1));
			if (tokens[i]->type == 5 || tokens[i]->type == 8)
				var->input_redir--;
			if (tokens[i]->type == 6 || tokens[i]->type == 7)
				var->output_redir--;
		}
		i++;
	}
	var->exit_code = run_command(var, tokens);
	return (restore_io(std_in, std_out, 0));
}
