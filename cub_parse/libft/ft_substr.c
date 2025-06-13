/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 13:47:45 by eralambo          #+#    #+#             */
/*   Updated: 2024/03/16 22:51:15 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str);

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	size_t			len_s;
	char			*res;

	i = 0;
	len_s = ft_strlen(s);
	if (start >= len_s)
		start = len_s;
	if (len > len_s - start)
		len = len_s - start;
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	if ((size_t)start >= len_s)
	{
		res[0] = '\0';
		return (res);
	}
	while ((s[start + i] != '\0') && (i < len))
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
