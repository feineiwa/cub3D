/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:22:43 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/13 19:07:36 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

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

int	check_id_rgb(const char *str, char id_rgb)
{
	while (white_space(*str))
		str++;
	if (*str != id_rgb || !white_space(*(str + 1)))
		return (0);
	return (1);
}

char	*get_rgb(const char *str, char id_rgb)
{
	while (white_space(*str))
		str++;
	if (*str != id_rgb)
		return (NULL);
	str++;
	if (!white_space(*str))
		return (NULL);
	while (white_space(*str))
		str++;
	return (ft_strtrim(str, " \t\n"));
}

int	is_valid_rgb_format(const char *str)
{
	int	i;
	int	digit_count;
	int	comma_count;

	i = 0;
	digit_count = 0;
	comma_count = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
		{
			digit_count++;
			if (digit_count > 3)
				return (0);
		}
		else if (str[i] == ',')
		{
			if (digit_count == 0)
				return (0);
			comma_count++;
			digit_count = 0;
		}
		else
			return (0);
		i++;
	}
	if (comma_count == 2 && digit_count > 0 && digit_count <= 3)
		return (1);
	return (0);
}

int	check_color_valid(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	if (g < 0 || g > 255)
		return (0);
	if (b < 0 || b > 255)
		return (0);
	return (1);
}

int	get_rgb_value(const char *str, int *r, int *g, int *b)
{
	char	**rgb_values;
	int		i;

	rgb_values = ft_split(str, ',');
	if (!rgb_values)
		return (0);
	i = 0;
	while (rgb_values[i])
		i++;
	if (i != 3)
		return (free_tab(rgb_values), 0);
	*r = ft_atoi(rgb_values[0]);
	*g = ft_atoi(rgb_values[1]);
	*b = ft_atoi(rgb_values[2]);
	free_tab(rgb_values);
	if (!check_color_valid(*r, *g, *b))
		return (0);
	return (1);
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

int	around_zero(char c)
{
	if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

int	count_player(char c)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

int	length_line_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

int	check_map_cell(char **map, int i, int j, t_vect v)
{
	if (map[j][i] != '1' && map[j][i] != '0' && map[j][i] != ' '
		&& !count_player(map[j][i]))
		return (0);
	if (map[j][i] == '0' || count_player(map[j][i]))
	{
		if (i == 0 || j == 0 || i == v.x - 1 || j == v.y - 1)
			return (0);
		if (!around_zero(map[j - 1][i]))
			return (0);
		if (!around_zero(map[j + 1][i]))
			return (0);
		if (!around_zero(map[j][i + 1]))
			return (0);
		if (!around_zero(map[j][i - 1]))
			return (0);
	}
	return (1);
}

int	validated_map(char **map)
{
	int		i;
	int		j;
	int		player_count;
	t_vect	v;

	player_count = 0;
	j = 0;
	v.y = length_line_map(map);
	v.x = ft_strlen(map[0]);
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (!check_map_cell(map, i, j, v))
				return (0);
			player_count += count_player(map[j][i]);
			i++;
		}
		j++;
	}
	if (player_count != 1)
		return (0);
	return (1);
}

void	free_text(t_text *text)
{
	if (text->no_path)
		free(text->no_path);
	if (text->so_path)
		free(text->so_path);
	if (text->we_path)
		free(text->we_path);
	if (text->ea_path)
		free(text->ea_path);
	if (text->ceiling)
		free(text->ceiling);
	if (text->floor)
		free(text->floor);
}
