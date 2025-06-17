/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validated_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:38:00 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/17 13:21:30 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"

static int	length_line_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

static int	check_map_cell(char **map, int i, int j, t_vect v)
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
