/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:01:57 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 00:07:30 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	count_door(char **map)
{
	int	j;
	int	i;
	int	count;

	j = 0;
	count = 0;
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == door_c || map[j][i] == door_o)
				count++;
			i++;
		}
		j++;
	}
	return (count);
}

void	init_door(t_door *door, t_cell cell, int pos_x, int pos_y)
{
	if (cell == door_c)
	{
		door->pos.x = pos_x;
		door->pos.y = pos_y;
		door->door_prog = 0.0;
	}
	else if (cell == door_o)
	{
		door->pos.x = pos_x;
		door->pos.y = pos_y;
		door->door_prog = 0.8;
	}
}

int	find_door(t_door *doors, int x, int y)
{
	int	i;

	i = 0;
	while (i < g_global()->door_count)
	{
		if (doors[i].pos.x == x && doors[i].pos.y == y)
			return (i);
		i++;
	}
	return (-1);
}

t_door	*handle_door(char **map)
{
	int		j;
	int		i;
	int		k;
	t_door	*doors;

	g_global()->door_count = count_door(map);
	doors = malloc(sizeof(t_door) * g_global()->door_count);
	if (doors == NULL)
		return (NULL);
	j = -1;
	k = 0;
	while (map[++j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == door_c)
				init_door(&doors[k++], door_c, i, j);
			else if (map[j][i] == door_o)
				init_door(&doors[k++], door_o, i, j);
			i++;
		}
	}
	return (doors);
}
