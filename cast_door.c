/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:56:20 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/07 13:09:32 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

t_image	*cast_ray_door(t_game *game, t_r *r)
{
	t_image *img;
	t_vector door_v;
	t_vector door_h;

	img = NULL;
	double door_v_dist;
	double	door_h_dist;
	int	door_index = find_door(game->doors, (int)floor(r->wall_hit.x / TILE_SIZE), (int)floor(r->wall_hit.y / TILE_SIZE));
	if (r->horizontal_dist < r->vertical_dist)
	{
		if (r->is_facing_up)
			r->s.y = - TILE_SIZE / 2;
		else if (r->is_facing_down)
			r->s.y = TILE_SIZE / 2;
		door_h.y = r->horiz_p.y + r->s.y;
		door_h.x = r->horiz_p.x +  (r->s.y / tan(r->ray_angle));
		door_v = r->vert_p;
		door_h_dist = distance_between_points(r->a, door_h);
		door_v_dist = r->vertical_dist;
		if (door_h_dist < door_v_dist)
			r->o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
		else
			r->o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
	}
	else
	{
		if (r->is_facing_left)
			r->s.x = - TILE_SIZE / 2;
		else if (r->is_facing_right)
			r->s.x = TILE_SIZE / 2;
		door_v.x = r->vert_p.x + r->s.x;
		door_v.y = r->vert_p.y + (r->s.x * tan(r->ray_angle));
		door_h = r->horiz_p;
		door_v_dist = distance_between_points(r->a, door_v);
		door_h_dist = r->horizontal_dist;
		if (door_h_dist < door_v_dist)
			r->o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
		else
			r->o = 1.0 - ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
	}
	if (r->o <= game->doors[door_index].door_prog)
	{
		t_vector	player;
		player = game->player.pos;
		if (r->horizontal_dist < r->vertical_dist)
		{
			r->found_horizontal = false;
			game->player.pos.x = (r->horiz_p.x / TILE_SIZE);
			game->player.pos.y = (r->horiz_p.y / TILE_SIZE);
			r->horiz_p = ray_cast_horizontal(game, r);
			r->horizontal_dist = set_dist(r->found_horizontal, r->a, r->horiz_p);
		}
		else
		{
			r->found_vertical = false;
			game->player.pos.x = (r->vert_p.x / TILE_SIZE);
			game->player.pos.y = (r->vert_p.y / TILE_SIZE);
			r->vert_p = ray_cast_vertical(game, r);
			r->vertical_dist = set_dist(r->found_vertical, r->a, r->vert_p);
		}
		game->player.pos = player;
		if (r->horizontal_dist < r->vertical_dist)
		{
			r->wall_hit = r->horiz_p;
			r->dist_wall = fabs(r->horizontal_dist);
			r->o = ((fmod(r->wall_hit.x, TILE_SIZE) / TILE_SIZE));
		}
		else
		{
			r->wall_hit = r->vert_p;
			r->dist_wall = fabs(r->vertical_dist);
			r->o = ((fmod(r->wall_hit.y, TILE_SIZE) / TILE_SIZE));
		}
		if (is_hit_door((int)floor(r->wall_hit.x), (int)floor(r->wall_hit.y), game->map, game->player))
		{
			if (r->horizontal_dist < r->vertical_dist)
			{
				if (r->is_facing_up)
					r->s.y = - TILE_SIZE / 2;
				else if (r->is_facing_down)
					r->s.y = TILE_SIZE / 2;
				door_h.y = r->horiz_p.y + r->s.y;
				door_h.x = r->horiz_p.x +  (r->s.y / tan(r->ray_angle));
				door_v = r->vert_p;
				door_h_dist = distance_between_points(r->a, door_h);
				door_v_dist = r->vertical_dist;
				if (door_h_dist < door_v_dist)
				{
					r->o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
					if (r->is_facing_up || r->is_facing_down)
						img = &game->pack.door[0];
					r->dist_wall = door_h_dist;
					r->wall_hit = door_h;
					if (r->is_facing_up)
						r->offset = img->w - (int)(r->o * img->w) - 1;
					else if (r->is_facing_down)
						r->offset = img->w - (int)(r->o * img->w) - 1;
				}
				else
				{
					r->o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
					if (r->is_facing_left || r->is_facing_right)
						img = &game->pack.door[1];
					r->dist_wall = door_v_dist;
					r->wall_hit = door_v;
					if (r->is_facing_left)
						r->offset = img->w - (int)(r->o * img->w) - 1;
					else if (r->is_facing_right)
						r->offset = (int)(r->o * img->w);
				}
			}
			else
			{
				if (r->is_facing_left)
					r->s.x = - TILE_SIZE / 2;
				else if (r->is_facing_right)
					r->s.x = TILE_SIZE / 2;
				door_v.x = r->vert_p.x + r->s.x;
				door_v.y = r->vert_p.y + (r->s.x * tan(r->ray_angle));
				door_h = r->horiz_p;
				door_v_dist = distance_between_points(r->a, door_v);
				door_h_dist = r->horizontal_dist;
				if (door_h_dist < door_v_dist)
				{
					r->o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
					if (r->is_facing_up || r->is_facing_down)
						img = &game->pack.door[1];
					r->dist_wall = door_h_dist;
					r->wall_hit = door_h;
					if (r->is_facing_up)
						r->offset = (int)(r->o * img->w);
					else if (r->is_facing_down)
						r->offset = img->w - (int)(r->o * img->w) - 1;
				}
				else
				{
					r->o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
					if (r->is_facing_left || r->is_facing_right)
						img = &game->pack.door[0];
					r->dist_wall = door_v_dist;
					r->wall_hit = door_v;
					if (r->is_facing_left)
						r->offset = (int)(r->o * img->w);
					else if (r->is_facing_right)
						r->offset = (int)(r->o * img->w);
				}
			}
		}
	}
	else
	{
		if (r->horizontal_dist < r->vertical_dist)
		{
			if (r->is_facing_up)
				r->s.y = - TILE_SIZE / 2;
			else if (r->is_facing_down)
				r->s.y = TILE_SIZE / 2;
			door_h.y = r->horiz_p.y + r->s.y;
			door_h.x = r->horiz_p.x +  (r->s.y / tan(r->ray_angle));
			door_v = r->vert_p;
			door_h_dist = distance_between_points(r->a, door_h);
			door_v_dist = r->vertical_dist;
			if (door_h_dist < door_v_dist)
			{
				r->o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
				if (r->is_facing_up || r->is_facing_down)
					img = &game->pack.door[0];
				r->dist_wall = door_h_dist;
				r->wall_hit = door_h;
				if (r->is_facing_up)
					r->offset = img->w - 1 - (int)((r->o - game->doors[door_index].door_prog) * img->w);
				else if (r->is_facing_down)
					r->offset = img->w - 1 - (int)((r->o - game->doors[door_index].door_prog) * img->w);
			}
			else
			{
				r->o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
				if (r->is_facing_left || r->is_facing_right)
					img = &game->pack.door[1];
				r->dist_wall = door_v_dist;
				r->wall_hit = door_v;
				if (r->is_facing_left)
					r->offset = img->w - (int)(r->o * img->w) - 1;
				else if (r->is_facing_right)
					r->offset = (int)(r->o * img->w);
			}
		}
		else
		{
			if (r->is_facing_left)
				r->s.x = - TILE_SIZE / 2;
			else if (r->is_facing_right)
				r->s.x = TILE_SIZE / 2;
			door_v.x = r->vert_p.x + r->s.x;
			door_v.y = r->vert_p.y + (r->s.x * tan(r->ray_angle));
			door_h = r->horiz_p;
			door_v_dist = distance_between_points(r->a, door_v);
			door_h_dist = r->horizontal_dist;
			if (door_h_dist < door_v_dist)
			{
				r->o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
				if (r->is_facing_up || r->is_facing_down)
					img = &game->pack.door[1];
				r->dist_wall = door_h_dist;
				r->wall_hit = door_h;
				if (r->is_facing_up)
					r->offset = (int)(r->o * img->w);
				else if (r->is_facing_down)
					r->offset = img->w - (int)(r->o * img->w) - 1;
			}
			else
			{
				r->o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
				if (r->is_facing_left || r->is_facing_right)
					img = &game->pack.door[0];
				r->dist_wall = door_v_dist;
				r->wall_hit = door_v;
				if (r->is_facing_left)
					r->offset = (int)((r->o + game->doors[door_index].door_prog) * img->w);
				else if (r->is_facing_right)
					r->offset = (int)((r->o + game->doors[door_index].door_prog) * img->w);
			}
		}
	}
	return (img);
}