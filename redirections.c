/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:43:11 by arissane          #+#    #+#             */
/*   Updated: 2024/10/11 18:49:37 by jmouette         ###   ########.fr       */
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
		i++;
	}
	return (count);
}

static void	restore_io(int std_in, int std_out)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close(std_in);
	close(std_out);
}

static int	redirect_input(char *tokens, int mode, char *error, int fd_target)
{
	char	*file;
	int		fd;

	file = ft_strtrim(tokens, "\"\'");
	if (!file)
	{
		perror(error);
		return (-1);
	}
	fd = open(file, mode, 0644);
	free(file);
	if (fd < 0)
	{
		perror(error);
		return (-1);
	}
	if (dup2(fd, fd_target) < 0)
	{
		perror("dup2 failed");
		close(fd);
		return (-1);
	}
	close(fd);
	return (1);
}

static int	process_redirection(t_token *token, t_token *next_token)
{
	int		check;
	char	*error_message;

	check = 0;
	if (token->type == REDIRECTION_LEFT)
	{	
		error_message = "Can't open file for reading";
		check = redirect_input(next_token->value,
				O_RDONLY, error_message, STDIN_FILENO);
	}
	else if (token->type == REDIRECTION_RIGHT)
	{	
		error_message = "Can't open file for writing";
		check = redirect_input(next_token->value,
				O_WRONLY | O_CREAT | O_TRUNC, error_message, STDOUT_FILENO);
	}
	else if (token->type == APPEND)
	{	
		error_message = "Can't open file for appending";
		check = redirect_input(next_token->value,
				O_WRONLY | O_CREAT | O_TRUNC, error_message, STDOUT_FILENO);
	}
	return (check);
}

int	handle_redirect(t_var *var, t_token **tokens)
{
	int	i;
	int	std_in;
	int	std_out;
	int	check;
	int	redirect_count;

	redirect_count = init(&std_in, &std_out, tokens, var);
	if (redirect_count == -1)
		return (-1);
	if (redirect_count == 0)
	{
		restore_io(std_in, std_out);
		return (0);
	}
	i = -1;
	while (tokens[i++] && var->exit_code != 1)
	{
		check = process_redirection(tokens[i], tokens[i + 1]);
		if (check == -1)
			break ;
		if (check == 1)
			var->exit_code = run_command(var, tokens);
	}
	restore_io(std_in, std_out);
	return (1);
}

/*
static int redirect_input(char *tokens, int mode, char *error, int fd_target)
{
	char	*file;
	int		fd;

	file = ft_strtrim(tokens, "\"\'");
	if (!file)
	{
		perror(error);
		return (-1);
	}
	fd = open(file, mode, 0644);
	free(file);
	if (fd < 0)
	{
		perror(error);
		return (-1);
	}
	if (dup2(fd, fd_target) < 0)
	{
		perror("dup2 failed");
		close(fd);
		return (-1);
	}
	close(fd);
	return (1);
}

int	handle_redirect(t_var *var, t_token **tokens)
{
	int	i;
	int	std_in;
	int	std_out;
	int	check;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	check = 0;

	int	k = 0;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type > 4) // check redirects
			k++;
		i++;
	}

	if (k > 0)
	{
		i = 0;
		while (tokens[i] && var->exit_code != 1)
		{
			if (tokens[i]->type == REDIRECTION_LEFT)
			{
				check = redirect_input(tokens[i + 1]->value,
				 O_RDONLY, "Can't open file for reading", STDIN_FILENO);
				if (check == -1)
					break ;
			}
			else if (tokens[i]->type == REDIRECTION_RIGHT)
			{
				check = redirect_input(tokens[i + 1]->value, 
				O_WRONLY | O_CREAT | O_TRUNC, 
				"Can't open file for writing", STDOUT_FILENO);
				if (check == -1)
					break ;
			}
			else if (tokens[i]->type == APPEND)
			{
				check = redirect_input(tokens[i + 1]->value, 
				O_WRONLY | O_CREAT | O_APPEND, 
				"Can't open file for appending", STDOUT_FILENO);
				if (check == -1)
					break ;
			}
			i++;
			if (check == 1)
				var->exit_code = run_command(var, tokens);
			check = 0;
		}
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		close (std_in);
		close (std_out);
		return (1);
	}
	close (std_in);
	close (std_out);
	return (0);
}*/
