/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:26:15 by arissane          #+#    #+#             */
/*   Updated: 2024/11/15 10:53:57 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//save the signal to a global variable so that we can update
//the exit code and write the same error as bash to stderr
void	handle_sigquit_exec(int sig)
{
	write(2, "Quit (core dumped)\n", 19);
	g_signal = sig;
}

//close stdin so that readline() will stop and the program
//will continue the normal loop
void	handle_sigint_heredoc(int sig)
{
	write(1, "\n", 1);
	g_signal = sig;
	close(STDIN_FILENO);
}

void	handle_sigint_exec(int sig)
{
	write(1, "\n", 1);
	g_signal = sig;
}

//Force the readline function stop waiting for more input
//and handle the different cases in the main
void	handle_sigint(int sig)
{
	g_signal = sig;
	close(STDIN_FILENO);
}
