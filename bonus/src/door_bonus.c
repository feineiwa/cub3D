/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:58:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/16 17:59:56 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

void	close_door(t_game *game, t_vector door_hit, t_vector *prev_door,
		int door_index)
{
	game->doors[door_index].door_prog += 0.1;
	game->player.key.door_check = true;
	if (game->doors[door_index].door_prog >= 1.0f)
	{
		game->doors[door_index].door_prog = 1.0f;
		game->player.key.door_check = false;
		g_global()->door_time = get_current_time();
		game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] = door_o;
		*prev_door = door_hit;
		g_global()->prev_index = door_index;
	}
}

void	open_door(t_game *game, t_vector door_hit, int door_index)
{
	game->doors[door_index].door_prog -= 0.1;
	if (game->doors[door_index].door_prog <= 0.0f)
	{
		game->doors[door_index].door_prog = 0.0f;
		game->player.key.door_check = false;
		g_global()->door_time = get_current_time();
		game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] = door_c;
		g_global()->prev_index = -1;
	}
}

void	close_automatic(t_game *game, t_vector prev_door)
{
	if (game->map[(int)floor(game->player.pos.y)] \
		[(int)floor(game->player.pos.x)] != door_o \
		&& g_global()->prev_index >= 0
		&& game->map[(int)floor(prev_door.y)][(int)floor(prev_door.x)] == door_o
		&& get_current_time() - g_global()->door_time >= 2000)
	{
		game->doors[g_global()->prev_index].door_prog -= 0.1;
		if (game->doors[g_global()->prev_index].door_prog <= 0.0f)
		{
			game->doors[g_global()->prev_index].door_prog = 0.0f;
			game->player.key.door_check = false;
			game->map[(int)floor(prev_door.y)] \
				[(int)floor(prev_door.x)] = door_c;
			g_global()->prev_index = -1;
			g_global()->door_time = get_current_time();
		}
	}
}

static void	cast_one_ray(t_game *game, t_r *r)
{
	r->ray_angle = normalize_angle(game->player.angle);
	compute_ray_orientation(r->ray_angle, r);
	r->horiz_p = ray_cast_horizontal(game, r);
	r->vert_p = ray_cast_vertical(game, r);
	r->horizontal_dist = set_dist(r->found_horizontal, r->a, r->horiz_p);
	r->vertical_dist = set_dist(r->found_vertical, r->a, r->vert_p);
	r->o = set_hit_and_dist_wall(r);
}

void	check_door_close(t_game *game)
{
	t_r				r;
	t_vector		door_hit;
	static t_vector	prev_door;
	int				door_index;

	init_ray(game, &r);
	cast_one_ray(game, &r);
	door_hit.x = r.wall_hit.x / TILE_SIZE;
	door_hit.y = r.wall_hit.y / TILE_SIZE;
	door_index = find_door(game->doors, (int)floor(door_hit.x),
			(int)floor(door_hit.y));
	if (r.dist_wall < 60 && game->player.key.door_check == true
		&& door_index >= 0)
	{
		if (game->map[(int)floor(door_hit.y)][(int)floor(door_hit.x)] == door_c
			&& g_global()->prev_index < 0)
			close_door(game, door_hit, &prev_door, door_index);
		else if (game->map[(int)floor(door_hit.y)] \
			[(int)floor(door_hit.x)] == door_o)
			open_door(game, door_hit, door_index);
	}
	else if (game->player.key.door_check == true)
		game->player.key.door_check = false;
	close_automatic(game, prev_door);
}
