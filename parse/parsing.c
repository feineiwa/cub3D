/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:26:15 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/20 15:04:47 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

char	ft_get_cell(char **map, int x, int y)
{
	if (!map || !*map)
		return (0);
	return (map[y][x]);
}

int	check_extension(char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len <= 4 || (len > 4 && s[len - 5] == '/'))
	{
		ft_putendl_fd("Error\nImpossible to lanch hiden file", 2);
		return (1);
	}
	if (ft_strcmp(s + (len - 4), ".cub"))
	{
		ft_putendl_fd("Error\nthe extension should be .cub", 2);
		return (1);
	}
	return (0);
}

int	check_blank_line_map(char *map)
{
	int	i;

	i = 0;
	if (!map)
		return (1);
	while (map[i])
	{
		if ((i == 0 || map[i - 1] == '\n') && (map[i] == 32 || map[i] == 9))
		{
			while (map[i] && (map[i] == 32 || map[i] == 9))
				i++;
			if ((map[i] && map[i] == '\n') || !map[i])
				return (1);
		}
		if (map[i] == '\n' && (map[i + 1] && map[i + 1] == '\n'))
			return (1);
		else if (map[i] == '\n' && !map[i + 1])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_pad_space(char *s, int len)
{
	char	*str;
	int		s_len;

	str = ft_calloc(sizeof(char), len + 1);
	if (!str)
		return (ft_strdup(""));
	s_len = ft_strlen(s);
	str = ft_memcpy(str, s, s_len);
	while (s_len < len)
		str[s_len++] = ' ';
	str[s_len] = 0;
	ft_free(s);
	return (str);
}

int	check_arround(char **map, int x, int y)
{
	if (ft_get_cell(map, x - 1, y) == ' ')
		return (0);
	if (ft_get_cell(map, x + 1, y) == ' ')
		return (0);
	if (ft_get_cell(map, x, y - 1) == ' ')
		return (0);
	if (ft_get_cell(map, x, y + 1) == ' ')
		return (0);
	return (1);
}

int	map_is_valid(char **map, char p, int width, int height)
{
	int	x;
	int	y;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == p || map[y][x] == ground)
			{
				if (x == 0 || y == 0)
					return (0);
				else if (x == width - 1
					|| y == height - 1)
					return (0);
				else if (!check_arround(map, x, y))
					return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

char	**get_width_and_height(char **s_map, char p)
{
	int	i;
	int	width;
	int	len;

	i = 0;
	width = 0;
	len = 0;
	while (s_map[i])
	{
		len = ft_strlen(s_map[i]);
		if (width < len)
			width = len;
		i++;
	}
	i = 0;
	while (s_map[i])
	{
		if ((int)ft_strlen(s_map[i]) < width)
			s_map[i] = ft_pad_space(s_map[i], width);
		i++;
	}
	if (!map_is_valid(s_map, p, width, i))
		return (ft_free_arr((void **)s_map), NULL);
	return (s_map);
}




char	**check_map_valid(char *map)
{
	int		i;
	int		player_count;
	char	**s_map;
	char	p;

	i = 0;
	player_count = 0;
	while (map[i])
	{
		if (map[i] != wall && map[i] != ground && map[i] != space
			&& map[i] != p_n && map[i] != p_s && map[i] != p_e && map[i] != p_w
			&& map[i] != '\n')
			return (NULL);
		if (map[i] == p_n || map[i] == p_s || map[i] == p_e || map[i] == p_w)
		{
			p = map[i];
			player_count++;
			if (player_count >= 2)
				return (NULL);
		}
		i++;
	}
	if (player_count == 0)
		return (NULL);
	s_map = ft_split(map, '\n');
	s_map = get_width_and_height(s_map, p);
	return (s_map);
}

int	check_color_value(int c)
{
	if (c < 0 || c > 255)
		return (1);
	return (0);
}

t_rgb	format_color_value(char *s, bool *err)
{
	t_rgb	color;
	int		i;
	char	**rgb;

	i = 0;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	if (s[0] == 0)
		return (color);
	while (s[i])
	{
		if (!ft_isdigit(s[i]) && s[i] != ',' && s[i] != ' ' && s[i] != '\t')
			return ((*err = 1), color);
		if (s[i] == ',' && (i == 0 || (s[i + 1] && s[i + 1] == ',') || s[i
				+ 1] == '\0'))
			return ((*err = 1), color);
		i++;
	}
	rgb = ft_split(s, ',');
	if (ft_array_lenght(rgb) != 3)
	{
		ft_free_arr((void **)rgb);
		return ((*err = 1), color);
	}
	color.r = ft_atoi(rgb[0]);
	color.g = ft_atoi(rgb[1]);
	color.b = ft_atoi(rgb[2]);
	if (check_color_value(color.r) || check_color_value(color.g)
		|| check_color_value(color.b))
	{
		ft_free_arr((void **)rgb);
		return ((*err = 1), color);
	}
	ft_free_arr((void **)rgb);
	return (color);
}

void	init_text(t_text *txt)
{
	txt->error = 0;
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
	txt->map = NULL;
}

t_text	ft_parsing(char *path_map)
{
	int		fd;
	char	*line;
	char	*trimed;
	char	*id;
	int		i;
	t_text	txt;
	int		count_id;
	char	*map;

	count_id = 0;
	init_text(&txt);
	map = NULL;
	if (check_extension(path_map))
		exit(1);
	fd = open(path_map, O_RDONLY);
	if (fd < 0)
	{
		ft_putendl_fd("Error", 2);
		perror(path_map);
		exit(1);
	}
	while (42)
	{
		line = get_next_line(fd);
		trimed = ft_strtrim(line, " \t\n");
		if (count_id < 6)
		{
			i = 0;
			while (trimed[i] && !ft_ispace(trimed[i]))
				i++;
			id = ft_substr(trimed, 0, i);
			i += ft_skip_space(trimed + i);
			if (!ft_strcmp(id, "NO") || !ft_strcmp(id, "WE") || !ft_strcmp(id,
					"EA") || !ft_strcmp(id, "SO") || !ft_strcmp(id, "F")
				|| !ft_strcmp(id, "C"))
			{
				if (!ft_strcmp(id, "NO"))
				{
					if (!txt.no_path)
					{
						txt.no_path = ft_substr(trimed, i, ft_strlen(trimed));
						count_id++;
					}
					else
						txt.error = 1;
				}
				else if (!ft_strcmp(id, "SO"))
				{
					if (!txt.so_path)
					{
						txt.so_path = ft_substr(trimed, i, ft_strlen(trimed));
						count_id++;
					}
					else
						txt.error = 1;
				}
				else if (!ft_strcmp(id, "EA"))
				{
					if (!txt.ea_path)
					{
						txt.ea_path = ft_substr(trimed, i, ft_strlen(trimed));
						count_id++;
					}
					else
						txt.error = 1;
				}
				else if (!ft_strcmp(id, "WE"))
				{
					if (!txt.we_path)
					{
						txt.we_path = ft_substr(trimed, i, ft_strlen(trimed));
						count_id++;
					}
					else
						txt.error = 1;
				}
				else if (!ft_strcmp(id, "C"))
				{
					if (!txt.ceiling)
					{
						txt.ceiling = ft_substr(trimed, i, ft_strlen(trimed));
						count_id++;
					}
					else
						txt.error = 1;
				}
				else if (!ft_strcmp(id, "F"))
				{
					if (!txt.floor)
					{
						txt.floor = ft_substr(trimed, i, ft_strlen(trimed));
						count_id++;
					}
					else
						txt.error = 1;
				}
			}
			else if (ft_strcmp(id, ""))
				txt.error = 1;
			ft_free(id);
		}
		else
		{
			if (!ft_strcmp(trimed, "") && count_id != 7)
				;
			else
			{
				if (map == NULL)
				{
					count_id++;
					map = ft_strdup("");
				}
				if (line != NULL)
					map = ft_strjoin_s1(map, line);
			}
		}
		ft_free(trimed);
		if (line == NULL)
			break ;
		ft_free(line);
		if (txt.error == 1)
			break ;
	}
	if (BONUS == 0)
	{
		if (txt.ceiling)
			txt.c = format_color_value(txt.ceiling, &txt.error);
		if (txt.floor)
			txt.f = format_color_value(txt.floor, &txt.error);
		txt.floor = ft_free(txt.floor);
		txt.ceiling = ft_free(txt.ceiling);
	}
	if (txt.error == 1)
	{
		while (1)
		{
			line = get_next_line(fd);
			if (line == NULL)
				break ;
			ft_free(line);
		}
	}
	else if (map)
	{
		if (check_blank_line_map(map))
			txt.error = 1;
		else
			txt.map = check_map_valid(map);
		if (txt.map == NULL)
			txt.error = 1;
		map = ft_free(map);
	}
	close(fd);
	if (count_id != 7 || txt.error == 1)
	{
		ft_free(map);
		ft_free_txt(&txt);
		return (ft_putendl_fd("Error\nMap Error", 2), (t_text){0});
	}
	return (txt);
}
