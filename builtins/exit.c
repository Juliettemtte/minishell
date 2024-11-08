/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:14:20 by jmouette          #+#    #+#             */
/*   Updated: 2024/11/08 13:27:17 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static long	get_numeric_exit_value(t_token **token, int i, int j, int is_neg)
{
	long	exit_code;

	exit_code = 0;
	while (token[j]->value[i] != '\0')
	{
		while (token[j]->value[i] == '"')
			i++;
		if (token[j]->value[i] == '\0')
			break ;
		if (!ft_isdigit(token[j]->value[i]))
		{
			write(2, "exit: numeric argument required\n", 32);
			exit(2);
		}
		exit_code = exit_code * 10 + (token[j]->value[i] - '0');
		i++;
	}
	if (is_neg)
		exit_code = 256 + (-exit_code % 256);
	else
		exit_code = exit_code % 256;
	return (exit_code);
}

int	my_exit(t_token **token)
{
	long	exit_code;
	int		is_negative;
	int		i;
	int		j;

	is_negative = 0;
	i = 0;
	j = find_command_index(token, "exit");
	j++;
	if (token[j] == NULL || token[j]->value == NULL)
		return (EXIT_SUCCESS);
	if (token[j + 1] != NULL && token[j + 1]->value != NULL)
	{
		write(2, "exit: too many arguments\n", 25);
		return (EXIT_FAILURE);
	}
	while (token[j]->value[i] == '+' || token[j]->value[i] == '"')
		i++;
	if (token[j]->value[i] == '-')
	{
		is_negative = 1;
		i++;
	}
	exit_code = get_numeric_exit_value(token, i, j, is_negative);
	return (exit_code);
}
