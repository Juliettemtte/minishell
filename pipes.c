/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:27:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/16 15:26:47 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**find_path(void)
{
	char	*path_env;
	char	*path_cpy;
	char	**full_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_cpy = ft_strdup(path_env);
	if (!path_cpy)
		return (NULL);
	full_path = ft_split(path_cpy, ':');
	free(path_cpy);
	if (!full_path)
		return (NULL);
	return (full_path);
}

char	*find_cmd_path(char *cmd)
{
	char	**full_path;
	char	*cmd_path;
	char	*tmp;
	int		i;

	full_path = find_path();
	i = 0;
	while (full_path[i] != NULL)
	{
		tmp = ft_strjoin(full_path[i], "/");
		if (!tmp)
			return (NULL);
		cmd_path = ft_strjoin(tmp, cmd);
		free (tmp);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
		{
			free_list(full_path);
			return (cmd_path);
		}
		i++;
	}
	free_list(full_path);
	return (NULL);
}

void handle_pipe(char **commands[], int num_commands)
{
	int		i;
	int		pipefd[2];
	int		prev_pipefd[2];
	pid_t	pid;

	i = 0;
	while (i < num_commands)
	{
		if (i < num_commands - 1)
			 if (pipe(pipefd) == -1)
				exit(EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) //enfant
		{
			if (i > 0) //not the 1st cmd -> STDIN to the previous pipe
			{
				dup2(prev_pipefd[0], STDIN_FILENO);
				close(prev_pipefd[0]);
				close(prev_pipefd[1]);
			}
			if (i < num_commands - 1) //not the last cmd -> STDOUT to the actual pipe
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			if (execve(commands[i][0], commands[i], NULL) == -1)
				exit(EXIT_FAILURE);
		}
		if (i > 0) //not the 1st cmd -> close the previous pipe
		{
			close(prev_pipefd[0]);
			close(prev_pipefd[1]);
		}
		if (i < num_commands - 1) //put actual pipe as previous
		{
			prev_pipefd[0] = pipefd[0];
			prev_pipefd[1] = pipefd[1];
		}
		i++;
	}
	i = 0;
	while (i < num_commands)
	{
		wait(NULL);
		i++;
	}
}

/*int main()
{
    // Exemple : ls | grep ".c" | sort
	char *cmd1[] = {"ls", "-l", NULL};
	char *cmd2[] = {"grep", ".c", NULL};
	char *cmd3[] = {"sort", NULL};

	cmd1[0] = find_cmd_path(cmd1[0]);
	cmd2[0] = find_cmd_path(cmd2[0]);
	cmd3[0] = find_cmd_path(cmd3[0]);
	char **commands[] = {cmd1, cmd2, cmd3};
    
	int num_commands = sizeof(commands) / sizeof(commands[0]);
    
	handle_pipe(commands, num_commands);
	return 0;
}*/
