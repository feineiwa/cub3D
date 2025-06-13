/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 13:57:26 by eralambo          #+#    #+#             */
/*   Updated: 2024/03/16 22:54:26 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int n)
{
	if (n < 0)
		n *= -1;
	return (n);
}

static int	ft_len_int(int n)
{
	int	temp;
	int	len;

	temp = n;
	len = 0;
	if (n == 0)
		return (1);
	else
	{
		while (temp != 0)
		{
			temp /= 10;
			len++;
		}
	}
	if (n < 0)
		len++;
	return (len);
}

char	*ft_itoa(int nbr)
{
	char	*str;
	int		len;
	int		i;

	len = ft_len_int(nbr);
	i = 0;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	if (nbr < 0)
		str[0] = '-';
	if (nbr == 0)
		str[0] = '0';
	i = len - 1;
	while (nbr != 0)
	{
		str[i--] = '0' + ft_abs(nbr % 10);
		nbr /= 10;
	}
	str[len] = '\0';
	return (str);
}
