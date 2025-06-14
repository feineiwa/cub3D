/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:19:34 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/14 09:18:19 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include <stdio.h>

void	init_text(t_text *txt)
{
	txt->no_path = NULL;
	txt->so_path = NULL;
	txt->ea_path = NULL;
	txt->we_path = NULL;
	txt->ceiling = NULL;
	txt->floor = NULL;
	txt->f.r = 0;
	txt->f.g = 0;
	txt->f.b = 0;
	txt->c.r = 0;
	txt->c.g = 0;
	txt->c.b = 0;
}

char	**append_line(char **lines, int count, char *new_line)
{
	char	**new_tab;
	int		i;

	i = 0;
	new_tab = malloc(sizeof(char *) * (count + 2));
	if (!new_tab)
		return (NULL);
	while (i < count)
	{
		new_tab[i] = lines[i];
		i++;
	}
	new_tab[count] = ft_strdup(new_line);
	new_tab[count + 1] = NULL;
	free(lines);
	return (new_tab);
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

void	get_path_nsewcf(char *input, t_text *txt, int *count, char *id_path)
{
	if (!ft_strncmp("NO", id_path, 2))
		txt->no_path = get_path(input, "NO");
	else if (!ft_strncmp("SO", id_path, 2))
		txt->so_path = get_path(input, "SO");
	else if (!ft_strncmp("EA", id_path, 2))
		txt->ea_path = get_path(input, "EA");
	else if (!ft_strncmp("WE", id_path, 2))
		txt->we_path = get_path(input, "WE");
	else if (!ft_strncmp("C", id_path, 1))
		txt->ceiling = get_rgb(input, 'C');
	else if (!ft_strncmp("F", id_path, 1))
		txt->floor = get_rgb(input, 'F');
	(*count)++;
}

int	get_map_setting(char **input, t_text *txt, int i)
{
	int	count;

	count = 0;
	while (input[i] && count < 6)
	{
		if (!white_line(input[i]))
		{
			if ((check_id_path(input[i], "NO")) && !txt->no_path)
				get_path_nsewcf(input[i], txt, &count, "NO");
			else if ((check_id_path(input[i], "SO")) && !txt->so_path)
				get_path_nsewcf(input[i], txt, &count, "SO");
			else if ((check_id_path(input[i], "EA")) && !txt->ea_path)
				get_path_nsewcf(input[i], txt, &count, "EA");
			else if ((check_id_path(input[i], "WE")) && !txt->we_path)
				get_path_nsewcf(input[i], txt, &count, "WE");
			else if ((check_id_rgb(input[i], 'C')) && !txt->ceiling)
				get_path_nsewcf(input[i], txt, &count, "C");
			else if ((check_id_rgb(input[i], 'F')) && !txt->floor)
				get_path_nsewcf(input[i], txt, &count, "F");
			else
				return (0);
		}
		i++;
	}
	return (i);
}

int	parsing(char **input, t_text *txt, char ***map)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	i = get_map_setting(input, txt, i);
	if (i == 0)
		return (0);
	while (white_line(input[i]))
		i++;
	while (input[i])
	{
		*map = append_line(*map, j, input[i]);
		j++;
		i++;
	}
	if (*map == NULL)
		return (0);
	return (1);
}
