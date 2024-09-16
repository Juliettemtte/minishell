/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_number.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:00:19 by jmouette          #+#    #+#             */
/*   Updated: 2024/04/29 10:00:21 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_number(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
		len += ft_string("-2147483648");
	else if (n < 0)
	{
		len += ft_putchar_len('-');
		if (len == -1)
			return (-1);
		len += ft_number(-n);
	}
	else if (n > 9)
	{
		len += ft_number(n / 10);
		if (len == -1)
			return (-1);
		len += ft_putchar_len((n % 10) + 48);
	}
	else
		len += ft_putchar_len(n + 48);
	if (len == -1)
		return (-1);
	return (len);
}

int	ft_unsigned_dec(unsigned int n)
{
	int	len;

	len = 0;
	if (n > 9)
	{
		len += ft_number(n / 10);
		if (len == -1)
			return (-1);
		len += ft_putchar_len((n % 10) + 48);
	}
	else
		len += ft_putchar_len(n + 48);
	if (len == -1)
		return (-1);
	return (len);
}

int	ft_number_hexa(unsigned long n, char c)
{
	int		len;
	char	*chars_lower;
	char	*chars_upper;

	len = 0;
	chars_lower = "0123456789abcdef";
	chars_upper = "0123456789ABCDEF";
	if (n >= 16)
	{
		len += ft_number_hexa(n / 16, c);
		if (len == -1)
			return (-1);
		len += ft_number_hexa(n % 16, c);
	}
	else
	{
		if (c == 'x')
			len += ft_putchar_len(chars_lower[n]);
		else if (c == 'X')
			len += ft_putchar_len(chars_upper[n]);
	}
	if (len == -1)
		return (-1);
	return (len);
}

int	ft_pointer(void *p)
{
	int		len;

	len = 0;
	if (p == 0)
	{
		len += ft_string("0x0");
		if (len == -1)
			return (-1);
	}
	else
	{
		len += ft_string("0x");
		if (len == -1)
			return (-1);
		len += ft_number_hexa((unsigned long)p, 'x');
		if (len == -1)
			return (-1);
	}
	return (len);
}
