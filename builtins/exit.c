/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:14:20 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/03 18:12:57 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	my_exit(char **arg)
{
	long	exit_code;
	int		is_negative;
	int		i;

	exit_code = 0;
	is_negative = 0;
	i = 0;
	if (arg[1] == NULL)
		exit(EXIT_SUCCESS);
	if (arg[2] != NULL)
	{
		printf("exit: too many arguments\n");
		exit(EXIT_FAILURE);
	}
	if (arg[1][0] == '-')
	{
		is_negative = 1;
		i++;
	}
	else if (arg[1][0] == '+')
		i++;
	while (arg[1][i] != '\0')
	{
		printf("nb %c\n", arg[1][i]);
		while (arg[1][i] == '"')
			i++;
		if (!ft_isdigit(arg[1][i]))
		{
			printf("exit: %s: numeric argument required\n", arg[1]);
			exit(EXIT_FAILURE);
		}
		exit_code = exit_code * 10 + (arg[1][i] - '0');
		i++;
	}
	if (is_negative)
		exit_code = 256 + (exit_code % 256);
	else
		exit_code = exit_code % 256;
	exit(exit_code);
}
