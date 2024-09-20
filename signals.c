/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:26:15 by arissane          #+#    #+#             */
/*   Updated: 2024/09/17 16:31:03 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	disable_control_chars()
{
	struct termios termios_p;

	tcgetattr(STDIN_FILENO, &termios_p); //take the current terminal settings and stores them in the struct
	termios_p.c_lflag &= ~(ECHOCTL); // clears the ECHOCTL flag -> the terminal will no longer display control characters
	tcsetattr(STDIN_FILENO, TCSANOW, &termios_p); // Apply the modified settings
}

void	init_signal(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	disable_control_chars();
}
