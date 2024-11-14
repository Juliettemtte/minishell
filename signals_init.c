/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:49:23 by arissane          #+#    #+#             */
/*   Updated: 2024/11/14 14:59:54 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	disable_control_chars(void)
{
	struct termios	termios_p;

	if (tcgetattr(STDIN_FILENO, &termios_p) == -1)
		return ;
	termios_p.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		return ;
}

void	init_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	disable_control_chars();
}
