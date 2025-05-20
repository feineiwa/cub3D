/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:58:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/05/20 06:28:06 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

// void	update_doors(t_game *game, double delta_time)
// {
// 	int	i;
// 	t_door *door;

// 	i = 0;
// 	while (i < g_global()->door_count)
// 	{
// 		door = &game->doors[i];
// 		if (door->is_opening && door->slide_progress < 1.0)
// 		{
// 			door->slide_progress += delta_time;
// 			if (door->slide_progress >= 1.0)
// 			{
// 				door->slide_progress = 1.0;
// 				door->is_opening = false;
// 			}
// 		}
// 		else if (door->is_closing && door->slide_progress > 0.0)
// 		{
// 			door->slide_progress -= delta_time;
// 			if (door->slide_progress <= 1.0)
// 			{
// 				door->slide_progress = 0.0;
// 				door->is_closing = false;
// 			}
// 		}
// 		i++;
// 	}
// }

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

void	init_door(t_door *door, t_cell	cell, int pos_x, int pos_y)
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
	int	j;
	int	i;
	int	k;
	t_door	*doors;
	
	g_global()->door_count = count_door(map);
	doors = malloc(sizeof(t_door) * g_global()->door_count);
	if (doors == NULL)
		return (NULL);
	j = 0;
	k = 0;
	while (map[j])
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
		j++;
	}
	return (doors);
}

void	check_door_close(t_game *game)
{
	t_vector	a;
	bool	is_facing_down;
	bool	is_facing_left;
	bool	is_facing_right;
	bool	is_facing_up;
	t_vector horiz_p = {0, 0};
	t_vector first_int = {0, 0};
	t_vector s = {0, 0};
	t_vector vert_p = {0, 0};
	double angle;

	a.x = game->player.pos.x * TILE_SIZE;
	a.y = game->player.pos.y * TILE_SIZE;
	angle = normalize_angle(game->player.angle);
	is_facing_down = false;
	is_facing_left = false;
	is_facing_right = false;
	is_facing_up = false;
	if (angle > 0 && angle < PI)
		is_facing_down = true;
	else
		is_facing_up = true;
	if (angle < 1.57079632679 || angle > 4.71238898038) // 0.5 * PI || 1.5 * PI
		is_facing_right = true;
	else
		is_facing_left = true;
	if (is_facing_up == 1)
	{
		first_int.y = (floor(game->player.pos.y) - 1e-6) * TILE_SIZE;
		s.y = - TILE_SIZE;
	}
	else if (is_facing_down == 1)
	{
		first_int.y = floor(game->player.pos.y) * TILE_SIZE + TILE_SIZE;
		s.y = TILE_SIZE;
	}
	
	first_int.x = a.x + (first_int.y - a.y) / tan(angle);
	s.x = s.y / tan(angle);
	t_vector next_horiz = {first_int.x, first_int.y};
	bool	found_horizontal = false;
	while (next_horiz.x < (game->player.w * TILE_SIZE) && next_horiz.x >= 0 && next_horiz.y < (game->player.h * TILE_SIZE) && next_horiz.y >= 0)
	{
		if (is_hit_door((int)floor(next_horiz.x), (int)floor(next_horiz.y), game->map, game->player))
		{
			found_horizontal = true;
			horiz_p = next_horiz;
			break ;
		}
		else
		{
			next_horiz.x += s.x;
			next_horiz.y += s.y;
		}
	}
	
	if (is_facing_right == 1)
	{
		first_int.x = floor(game->player.pos.x) * TILE_SIZE + TILE_SIZE;
		s.x = TILE_SIZE;
	}
	else if (is_facing_left == 1)
	{
		first_int.x = (floor(game->player.pos.x) - 1e-6) * TILE_SIZE;
		s.x = -TILE_SIZE;
	}
	first_int.y = a.y + ((first_int.x - a.x) * tan(angle));
	s.y = s.x * tan(angle);
	
	t_vector next_vert = {first_int.x, first_int.y};
	bool	found_vertical = false;
	while (next_vert.x < (game->player.w * TILE_SIZE)  && next_vert.x >= 0 && next_vert.y < (game->player.h * TILE_SIZE)  && next_vert.y >= 0)
	{
		if (is_hit_door((int)floor(next_vert.x), (int)floor(next_vert.y), game->map, game->player))
		{
			found_vertical = true;
			vert_p.x = next_vert.x;
			vert_p.y = next_vert.y;
			break ;
		}
		else
		{
			next_vert.x += s.x;
			next_vert.y += s.y;
		}
	}
	double horizontal_dist = 0;
	double vertical_dist = 0;
	if (found_horizontal == true)
		horizontal_dist = distance_between_points(a, horiz_p);
	else
		horizontal_dist = __DBL_MAX__;
	if (found_vertical == true)
		vertical_dist = distance_between_points(a, vert_p);
	else
		vertical_dist = __DBL_MAX__;
	double		dist_door = 0;
	t_vector	door_hit;
	if (horizontal_dist < vertical_dist)
	{
		dist_door = fabs(horizontal_dist);
		door_hit.x = horiz_p.x / TILE_SIZE;
		door_hit.y = horiz_p.y / TILE_SIZE;
	}
	else
	{
		dist_door = fabs(vertical_dist);
		door_hit.x = vert_p.x / TILE_SIZE;
		door_hit.y = vert_p.y / TILE_SIZE;
	}
	static	size_t door_time = 0;
	static t_vector	prev_door;
	static	int		prev_index = -1;
	int	door_index = find_door(game->doors, (int)floor(door_hit.x), (int)floor(door_hit.y));
	static bool	allow_to_open = false;
	if (dist_door < 60 && game->player.key.door_check == true && door_index >= 0)
	{
		if (game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] == door_c && prev_index < 0)
		{
			game->doors[door_index].door_prog += 0.1;
			g_global()->updated = true;
			game->player.key.door_check = true;
			if (game->doors[door_index].door_prog >= 1.0f)
			{
				game->doors[door_index].door_prog = 1.0f;
				game->player.key.door_check = false;
				door_time = get_current_time();
				game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] = door_o;
				prev_door = door_hit;
				prev_index = door_index;
			}
		}
		else if(game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] == door_o)
		{
			game->doors[door_index].door_prog -= 0.1;
			g_global()->updated = true;
			if (game->doors[door_index].door_prog <= 0.0f)
			{
				game->doors[door_index].door_prog = 0.0f;
				game->player.key.door_check = false;
				door_time = get_current_time();
				game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] = door_c;
				prev_index = -1;
			}
		}
	}
	else
		game->player.key.door_check = false;
	if (game->map[(int)floor(game->player.pos.y)][(int)floor(game->player.pos.x)] != door_o && prev_index >= 0 && game->map[(int)floor(prev_door.y)][(int)floor(prev_door.x)] == door_o && get_current_time() - door_time >= 2000)
	{
		game->doors[prev_index].door_prog -= 0.1;
		g_global()->updated = true;
		if (game->doors[prev_index].door_prog <= 0.0f)
		{
			game->doors[prev_index].door_prog = 0.0f;
			game->player.key.door_check = false;
			game->map[(int)floor(prev_door.y)][(int)floor(prev_door.x)] = door_c;
			prev_index = -1;
			door_time = get_current_time();
		}
	}
}
