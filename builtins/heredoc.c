/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:10:36 by arissane          #+#    #+#             */
/*   Updated: 2024/11/11 13:43:34 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*parse_line(t_var *var, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] != ' '
			&& line[i + 1] != '\"' && line[i + 1] != '\'' && line[i + 1] != '$')
			line = get_env_value(var, line, i);
		i++;
	}
	return (line);
}

//dup2 the read end of the pipe that was saved to var->heredoc_fds to STDIN
int	redirect_heredoc(t_var *var, t_token *token)
{
	if (var->input_redir == 1)
	{
		if (dup2(var->heredoc_fds[token->heredoc_index], STDIN_FILENO) < 0)
		{
			perror("dup2 failed");
			return (1);
		}
	}
	return (0);
}

//start a loop of readline until delimiter is encountered and write to
//var->heredoc_fds[]
static int	create_heredoc(t_var *var, char *delimiter, int heredoc_index)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (1);
	var->heredoc_fds[heredoc_index] = pipe_fd[0];
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		line = parse_line(var, line);
		ft_putstr_fd(line, pipe_fd[1]);
		write(pipe_fd[1], "\n", 1);
		if (line)
			free(line);
	}
	if (line)
		free(line);
	close(pipe_fd[1]);
	if (g_signal == SIGINT)
		return (1);
	return (0);
}

//read input to each heredoc fd
int	handle_heredoc(t_var *var, t_token *tokens)
{
	int	i;
	char	*delimiter;
//	int	saved_stdin;

	i = 0;
//	saved_stdin = dup(STDIN_FILENO);
	if (tokens)
	{
		while (tokens[i].value)
		{
			if (tokens[i].type == HEREDOC)
			{
				//signal(SIGINT, handle_sigint_heredoc);
				delimiter = remove_quotes(var, tokens[i + 1].value);
				if (create_heredoc(var, delimiter,
						tokens[i].heredoc_index) == 1)
				{
					//dup2(saved_stdin, STDIN_FILENO);
					//close(saved_stdin);
					return (1);
				}
				//signal(SIGINT, handle_sigint);
			}
			i++;
		}
	}
	//close(saved_stdin);
	return (0);
}

int	init_heredoc_fds(t_var *var)
{
	int	i;

	i = 0;
	if (var->heredoc_count > 0)
	{
		var->heredoc_fds = malloc(sizeof(int) * (var->heredoc_count + 1));
		if (!var->heredoc_fds)
			return (1);
		while (i < var->heredoc_count)
		{
			var->heredoc_fds[i] = -1;
			i++;
		}
		var->heredoc_fds[i] = -1;
	}
	return (0);
}
