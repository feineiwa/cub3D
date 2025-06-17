/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:22:43 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/17 13:22:23 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse_bonus.h"

int	white_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f')
		return (1);
	return (0);
}

int	white_line(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (!white_space(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	around_zero(char c)
{
	if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W'
		|| c == 'D' || c == 's')
		return (1);
	return (0);
}

int	count_player(char c)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

char	**map_to_char(char *path_map)
{
	int		fd;
	char	*line;
	char	**res;
	int		count;

	res = NULL;
	count = 0;
	fd = open(path_map, O_RDONLY);
	if (fd < 0)
	{
		ft_putendl_fd("Error\nCannot open the file", 2);
		exit(1);
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		res = append_line(res, count, line);
		free(line);
		count++;
	}
	return (close(fd), res);
}
