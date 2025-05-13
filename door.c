/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:58:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/05/13 16:47:50 by frahenin         ###   ########.fr       */
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
		door->door_prog = 1.0;
	}
	else if (cell == door_o)
	{
		door->pos.x = pos_x;
		door->pos.y = pos_y;
		door->door_prog = 0.0;
	}
}

t_door	*handle_door(char **map)
{ 
	int	j;
	int	i;
	int	k;
	t_door	*doors;
	
	doors = malloc(sizeof(t_door) * count_door(map));
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
	if (dist_door < 60 && game->player.key.door_check == true)
	{
		if (game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] == door_c)
		{
			g_global()->door_prog += 0.1;
			g_global()->updated = true;
			game->player.key.door_check = true;
			if (g_global()->door_prog >= 0.8f)
			{
				g_global()->door_prog = 0.8f;
				game->player.key.door_check = false;
			}
			if (g_global()->door_prog == 0.8f)
				game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] = door_o;
		}
		else if(game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] == door_o)
		{
			
			g_global()->door_prog -= 0.1;
			if (g_global()->door_prog <= 0.0f)
			{
				g_global()->door_prog = 0.0f;
				game->player.key.door_check = false;
			}
			if (g_global()->door_prog == 0.0f)
				game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] = door_c;
		}
	}
}

// if (is_hit_door((int)floor(wall_hit.x), (int)floor(wall_hit.y), game->map, game->player))
			// {
			// 	if (horizontal_dist < vertical_dist)
			// 	{
			// 		if (is_facing_up)
			// 			s.y = - TILE_SIZE / 2;
			// 		else if (is_facing_down)
			// 			s.y = TILE_SIZE / 2;
			// 		door_h.y = horiz_p.y + s.y;
			// 		door_h.x = horiz_p.x +  (s.y / tan(ray_angle));
			// 		door_v = vert_p;
			// 		door_h_dist = distance_between_points(a, door_h);
			// 		door_v_dist = vertical_dist;
			// 		if (door_h_dist < door_v_dist)
			// 		{
			// 			o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
			// 			if (is_facing_up || is_facing_down)
			// 				img = &game->pack.door[0];
			// 			dist_wall = door_h_dist;
			// 			wall_hit = door_h;
			// 			if (is_facing_up)
			// 				offset = (int)((g_global()->door_prog * img->w) - (o * img->w));
			// 			else if (is_facing_down)
			// 				offset = (int)(g_global()->door_prog * img->w) - (1.0 - o * img->w);
			// 		}
			// 		else
			// 		{
			// 			o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
			// 			if (is_facing_left || is_facing_right)
			// 				img = &game->pack.door[1];
			// 			dist_wall = door_v_dist;
			// 			wall_hit = door_v;
			// 			if (is_facing_left)
			// 				offset = img->w - (int)(o * img->w) - 1;
			// 			else if (is_facing_right)
			// 				offset = (int)(o * img->w);
			// 		}
			// 	}
			// 	else
			// 	{
			// 		if (is_facing_left)
			// 			s.x = - TILE_SIZE / 2;
			// 		else if (is_facing_right)
			// 			s.x = TILE_SIZE / 2;
			// 		door_v.x = vert_p.x + s.x;
			// 		door_v.y = vert_p.y + (s.x * tan(ray_angle));
			// 		door_h = horiz_p;
			// 		door_v_dist = distance_between_points(a, door_v);
			// 		door_h_dist = horizontal_dist;
			// 		if (door_h_dist < door_v_dist)
			// 		{
			// 			o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
			// 			if (is_facing_up || is_facing_down)
			// 				img = &game->pack.door[1];
			// 			dist_wall = door_h_dist;
			// 			wall_hit = door_h;
			// 			if (is_facing_up)
			// 				offset = (int)(o * img->w);
			// 			else if (is_facing_down)
			// 				offset = img->w - (int)(o * img->w) - 1;
			// 		}
			// 		else
			// 		{
			// 			o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
			// 			if (is_facing_left || is_facing_right)
			// 				img = &game->pack.door[0];
			// 			dist_wall = door_v_dist;
			// 			wall_hit = door_v;
			// 			if (is_facing_left)
			// 				offset = (int)(g_global()->door_prog * img->w) - (1.0 - o * img->w);
			// 			else if (is_facing_right)
			// 				offset = (int)((g_global()->door_prog * img->w) - (o * img->w));
			// 		}
			// 	}
			// }
