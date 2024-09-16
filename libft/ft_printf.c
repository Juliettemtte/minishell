/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 12:18:26 by jmouette          #+#    #+#             */
/*   Updated: 2024/05/22 10:39:12 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_value(char value, va_list args)
{
	int	len;

	len = 0;
	if (value == 'c')
		len += ft_putchar_len((char) va_arg(args, int));
	else if (value == 's')
		len += ft_string((char *) va_arg(args, char *));
	else if (value == 'p')
		len += ft_pointer((void *) va_arg(args, void *));
	else if (value == 'd' || value == 'i')
		len += ft_number((int) va_arg(args, int));
	else if (value == 'u')
		len += ft_unsigned_dec((unsigned int) va_arg(args, unsigned int));
	else if (value == 'x')
		len += ft_number_hexa((unsigned int) va_arg(args, unsigned int), 'x');
	else if (value == 'X')
		len += ft_number_hexa((unsigned int) va_arg(args, unsigned int), 'X');
	else if (value == '%')
		len += ft_putchar_len('%');
	else
		return (-1);
	return (len);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		len;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%' && str[i + 1] != '\0')
		{
			len += check_value(str[i + 1], args);
			i++;
		}
		else if (str[i] != '%')
			len += ft_putchar_len(str[i]);
		if (len == -1)
			return (-1);
		i++;
	}
	va_end(args);
	return (len);
}

/*#include <stdio.h>
int main()
{
	ft_printf("Hello World %");
    return 0;
}*/
