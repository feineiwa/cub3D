/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:28:12 by eralambo          #+#    #+#             */
/*   Updated: 2024/03/16 22:54:19 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		if (*((unsigned char *)s1 + i)
			== *((unsigned char *)s2 + i))
			i++;
		else
			return ((*((unsigned char *)s1 + i))
				- *((unsigned char *)s2 + i));
	}
	return (0);
}
