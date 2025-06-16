/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:35:25 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/16 14:43:59 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse_bonus.h"

int	ft_array_lenght(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	replace_new_line_space(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			s[i] = ' ';
		i++;
	}
}

char	*ft_pad_space(char *map, int w)
{
	char	*s;
	int		len;

	len = (int)ft_strlen(map);
	s = (char *)ft_calloc(sizeof(char), w + 1);
	if (!s)
		return (map);
	ft_strlcpy(s, map, len + 1);
	if (len < w)
	{
		while (len < w)
			s[len++] = ' ';
		s[len] = 0;
	}
	replace_new_line_space(s);
	free(map);
	return (s);
}

char	*get_path(const char *str, char *id_path)
{
	while (white_space(*str))
		str++;
	if (*str != *id_path || *(str + 1) != *(id_path + 1))
		return (NULL);
	str += 2;
	if (!white_space(*str))
		return (NULL);
	while (white_space(*str))
		str++;
	return (ft_strtrim(str, " \t\n"));
}

void	padding_space(char **map)
{
	int	len;
	int	w;
	int	i;

	w = 0;
	i = 0;
	while (map[i])
	{
		len = (int)ft_strlen(map[i]);
		if (w < len)
			w = len;
		i++;
	}
	i = 0;
	while (map[i])
	{
		map[i] = ft_pad_space(map[i], w);
		i++;
	}
}
