/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:26:15 by arissane          #+#    #+#             */
/*   Updated: 2024/10/19 15:45:27 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int sig)
{
	if (sig == SIGINT)//ctrl-C
	{
		if (g_status == HEREDOC)//if inside heredoc and we receive SIGINT, perform SIGQUIT to terminate heredoc
			handle_signal(SIGQUIT);
		else if (g_status == EXECUTE)//if executing a process, newline and replace current line with empty
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (sig == SIGQUIT)
	{
		write(1, "\n", 1);
		clear_history();//clear the readline buffer
		signal(sig, SIG_DFL);//restore the default function of sig
		kill(getpid(), sig);//send sig to the current process->SIGQUIT with default result
	}
}

static void	disable_control_chars(void)
{
	struct termios	termios_p;

	tcgetattr(STDIN_FILENO, &termios_p);
	termios_p.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &termios_p);
}

void	init_signal(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);//ignore by default and change behaviour if inside heredoc or another process
	disable_control_chars();
}
