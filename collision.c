/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:06:39 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/05 08:18:23 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

bool is_exceeded(t_vector move, t_player player)
{
	t_vect top_left;
	t_vect top_right;
	t_vect bottom_left;
	t_vect bottom_right;
	double	collider = 0.1;

	top_left.x = (int)(move.x - collider);
	top_left.y = (int)(move.y - collider);
	top_right.x = (int)(move.x + collider);
	top_right.y = (int)(move.y - collider);
	bottom_left.x = (int)(move.x - collider);
	bottom_left.y = (int)(move.y + collider);
	bottom_right.x = (int)(move.x + collider);
	bottom_right.y = (int)(move.y + collider);
	if (top_left.x >= player.w || top_left.x < 0 || top_left.y < 0 || top_left.y >= player.h)
		return (true);
	if (top_right.x >= player.w || top_right.x < 0 || top_right.y < 0 || top_right.y >= player.h)
		return (true);
	if (bottom_left.x >= player.w || bottom_left.x < 0 || bottom_left.y < 0 || bottom_left.y >= player.h)
		return (true);
	if (bottom_right.x >= player.w || bottom_right.x < 0 || bottom_right.y < 0 || bottom_right.y >= player.h)
		return (true);
	return(false);
}

bool	is_hit_wall(int tile_x, int tile_y, char **map)
{
	if (map[tile_y][tile_x] == wall || map[tile_y][tile_x] == door_c)
		return (true);
	return (false);	
}

bool	is_collision(t_vector move, t_player player, char **map)
{
	t_vect top_left;
	t_vect top_right;
	t_vect bottom_left;
	t_vect bottom_right;
	double	collider = 0.1;

	top_left.x = (int)(move.x - collider);
	top_left.y = (int)(move.y - collider);
	top_right.x = (int)(move.x + collider);
	top_right.y = (int)(move.y - collider);
	bottom_left.x = (int)(move.x - collider);
	bottom_left.y = (int)(move.y + collider);
	bottom_right.x = (int)(move.x + collider);
	bottom_right.y = (int)(move.y + collider);
	if (is_hit_wall(top_left.x, top_left.y, map) ||
		is_hit_wall(top_right.x, top_right.y, map) ||
		is_hit_wall(bottom_left.x, bottom_left.y, map) ||
		is_hit_wall(bottom_right.x, bottom_right.y, map))
		return (true);
	return (false);
}

bool	check_collision(t_vector move, t_player player, char **map)
{
	if (!is_exceeded(move, player) && !is_collision(move, player, map))
		return (false);
	return (true);
}

bool	is_wall_door(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == wall || map[y1][x1] == door_c)
		return (true);
	return (false);
}