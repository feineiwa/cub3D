/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validated_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:19:24 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/15 14:52:00 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"

int	check_arguments(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3d <map.cub>\n", 2);
		return (0);
	}
	if (ft_strlen(argv[1]) <= 4 || ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4,
			".cub", 4) != 0)
	{
		ft_putstr_fd("Error\nInvalid file extension. Expected .cub\n", 2);
		return (0);
	}
	if (ft_strncmp(argv[1] + ft_strlen(argv[1]) - 5, "/.cub", 5) == 0)
	{
		ft_putstr_fd("Error\nInvalid file name.\n", 2);
		return (0);
	}
	return (1);
}

int	is_validated_color(t_text *txt)
{
	if (!(is_valid_rgb_format(txt->ceiling)) || !get_rgb_value(txt->ceiling,
			&txt->c.r, &txt->c.g, &txt->c.b))
	{
		ft_putendl_fd("Error\ncheck color", 2);
		return (0);
	}
	if (!(is_valid_rgb_format(txt->floor))
		|| !get_rgb_value(txt->floor, &txt->f.r, &txt->f.g, &txt->f.b))
	{
		ft_putendl_fd("Error\ncheck color", 2);
		return (0);
	}
	return (1);
}

int	check_blank_line_map(char **map)
{
	int	j;

	j = 0;
	while (map[j])
	{
		if (white_line(map[j]))
			return (1);
		j++;
	}
	return (0);
}
