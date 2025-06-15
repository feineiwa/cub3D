/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:46:36 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 14:46:59 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"

int	check_id_path(const char *str, char *id_path)
{
	while (white_space(*str))
		str++;
	if (*str != *id_path || *(str + 1) != *(id_path + 1) || !white_space(*(str
				+ 2)))
		return (0);
	return (1);
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

char	*ft_strdup_mod(char *str, int max_len)
{
	char	*new_str;
	int		i;

	new_str = malloc(sizeof(char) * (max_len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < max_len)
	{
		if (str[i])
			new_str[i] = str[i];
		else
			new_str[i] = ' ';
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	**get_map(char **str)
{
	int		i;
	char	**map;
	size_t	max_len;

	i = 0;
	max_len = 0;
	while (str[i])
	{
		if (ft_strlen(str[i]) > max_len)
			max_len = ft_strlen(str[i]);
		i++;
	}
	map = malloc(sizeof(char *) * (max_len + 1));
	if (!map)
		return (NULL);
	i = 0;
	while (str[i])
	{
		map[i] = ft_strdup_mod(str[i], max_len);
		i++;
	}
	map[i] = NULL;
	return (map);
}
