/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 04:11:11 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/27 09:37:17 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	calc_len(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		len ++;
	}
	return (len);
}

char	*expt(int n)
{
	char	*r;

	r = NULL;
	if (n == 0)
	{
		r = malloc (2);
		r[0] = '0';
		r[1] = '\0';
		return (r);
	}
	if (n == -2147483648)
	{
		r = malloc(12);
		if (r == NULL)
			return (NULL);
		r = ft_strdup("-2147483648");
	}
	return (r);
}

char	*it_neg(int n)
{
	int		len;
	char	*r;

	n = -n;
	len = calc_len(n);
	r = malloc (len + 2);
	if (r == NULL)
		return (NULL);
	r[0] = '-';
	r[len + 1] = '\0';
	while (len > 0)
	{
		r[len] = (n % 10) + '0';
		n = n / 10;
		len --;
	}
	return (r);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*r;

	if (n == 0 || n == -2147483648)
		return (expt(n));
	if (n < 0)
		return (it_neg(n));
	len = calc_len(n);
	r = malloc (len +1);
	if (r == NULL)
		return (NULL);
	r [len] = '\0';
	while (len > 0)
	{
		r[len - 1] = (n % 10) + '0';
		n = n / 10;
		len --;
	}
	return (r);
}
