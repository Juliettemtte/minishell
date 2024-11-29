/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:27:40 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/23 14:09:15 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_redirect(int i, int nb_cmd, int prev_pipefd[2], int pipefd[2])
{
	if (i > 0)
	{
		if (dup2(prev_pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		close(prev_pipefd[0]);
		close(prev_pipefd[1]);
	}
	if (i < nb_cmd - 1)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

static void	wait_for_commands(t_var *var, pid_t last_pid)
{
	int		status;
	pid_t	pid;

	var->exit_code = 0;
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		var->exit_code = WEXITSTATUS(status);
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
			break ;
		pid = waitpid(-1, &status, 0);
	}
	if (var->pipes == 0 && (ft_strcmp(var->cmd_list[0], "exit") == 0))
		var->exit_code = -2;
}

static void	handle_parent_process(int num_commands, int *prev_pipefd,
		int *pipefd, int i)
{
	signal(SIGINT, SIG_IGN);
	if (i > 0)
	{
		close(prev_pipefd[0]);
		close(prev_pipefd[1]);
	}
	if (i < num_commands - 1)
	{
		prev_pipefd[0] = pipefd[0];
		prev_pipefd[1] = pipefd[1];
	}
}

static void	handle_child_process(t_var *var, t_token **token_group)
{
	int	check;

	check = handle_redirect(var, token_group, 0);
	if (check == -2)
	{
		check = run_command(var, token_group);
		close_heredoc_fds(var);
		free_env(&var->envp);
		free_shell(var);
		exit(check);
	}
	close_heredoc_fds(var);
	free_env(&var->envp);
	free_shell(var);
	exit(check);
}

void	handle_pipe(t_token ***token_groups, int num_commands, t_var *var)
{
	int		i;
	int		pipefd[2];
	int		prev_pipefd[2];
	pid_t	pid;

	i = 0;
	close(var->fd_in);
	var->fd_in = -1;
	while (i < num_commands)
	{
		if (i < num_commands - 1 && pipe(pipefd) == -1)
			exit(1);
		pid = fork();
		if (pid == -1)
			exit(handle_pipe_error(pipefd));
		if (pid == 0)
		{
			setup_redirect(i, num_commands, prev_pipefd, pipefd);
			handle_child_process(var, token_groups[i]);
		}
		else
			handle_parent_process(num_commands, prev_pipefd, pipefd, i);
		i++;
	}
	wait_for_commands(var, pid);
}
