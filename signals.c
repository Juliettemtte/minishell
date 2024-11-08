/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:26:15 by arissane          #+#    #+#             */
/*   Updated: 2024/11/08 10:49:57 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigquit(int sig)
{
	g_signal = sig;
}

void	handle_sigint_heredoc(int sig)
{
	write(1, "\n", 1);
	g_signal = sig;
	close(STDIN_FILENO);
}

void	handle_sigint_exec(int sig)
{
	g_signal = sig;
}

void	handle_sigint(int sig)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)sig;
}
