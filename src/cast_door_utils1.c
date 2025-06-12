/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_door_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 01:00:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 01:01:06 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	slide_door_north_and_south(t_r *r, t_d *d)
{
	if (r->is_facing_up)
		r->s.y = -TILE_SIZE / 2;
	else if (r->is_facing_down)
		r->s.y = TILE_SIZE / 2;
	d->door_h.y = r->horiz_p.y + r->s.y;
	d->door_h.x = r->horiz_p.x + (r->s.y / tan(r->ray_angle));
	d->door_v = r->vert_p;
	d->door_h_dist = distance_between_points(r->a, d->door_h);
	d->door_v_dist = r->vertical_dist;
}

void	slide_door_east_and_west(t_r *r, t_d *d)
{
	if (r->is_facing_left)
		r->s.x = -TILE_SIZE / 2;
	else if (r->is_facing_right)
		r->s.x = TILE_SIZE / 2;
	d->door_v.x = r->vert_p.x + r->s.x;
	d->door_v.y = r->vert_p.y + (r->s.x * tan(r->ray_angle));
	d->door_h = r->horiz_p;
	d->door_v_dist = distance_between_points(r->a, d->door_v);
	d->door_h_dist = r->horizontal_dist;
}

t_image	*get_door_image_north_and_south(t_game *game, t_r *r, t_d *d,
		t_image *img)
{
	if (d->door_h_dist < d->door_v_dist)
	{
		r->o = ((fmod(d->door_h.x, TILE_SIZE) / TILE_SIZE));
		if (r->is_facing_up || r->is_facing_down)
			img = &game->pack.door[0];
		r->dist_wall = d->door_h_dist;
		r->wall_hit = d->door_h;
		if (r->is_facing_up)
			r->offset = img->w - 1 - (int)((r->o
						- game->doors[d->door_index].door_prog) * img->w);
		else if (r->is_facing_down)
			r->offset = img->w - 1 - (int)((r->o
						- game->doors[d->door_index].door_prog) * img->w);
		return (img);
	}
	r->o = ((fmod(d->door_v.y, TILE_SIZE) / TILE_SIZE));
	if (r->is_facing_left || r->is_facing_right)
		img = &game->pack.door[1];
	r->dist_wall = d->door_v_dist;
	r->wall_hit = d->door_v;
	if (r->is_facing_left)
		r->offset = img->w - (int)(r->o * img->w) - 1;
	else if (r->is_facing_right)
		r->offset = (int)(r->o * img->w);
	return (img);
}

t_image	*get_door_image_east_and_west(t_game *game, t_r *r, t_d *d,
		t_image *img)
{
	if (d->door_h_dist < d->door_v_dist)
	{
		r->o = ((fmod(d->door_h.x, TILE_SIZE) / TILE_SIZE));
		if (r->is_facing_up || r->is_facing_down)
			img = &game->pack.door[1];
		r->dist_wall = d->door_h_dist;
		r->wall_hit = d->door_h;
		if (r->is_facing_up)
			r->offset = (int)(r->o * img->w);
		else if (r->is_facing_down)
			r->offset = img->w - (int)(r->o * img->w) - 1;
		return (img);
	}
	r->o = ((fmod(d->door_v.y, TILE_SIZE) / TILE_SIZE));
	if (r->is_facing_left || r->is_facing_right)
		img = &game->pack.door[0];
	r->dist_wall = d->door_v_dist;
	r->wall_hit = d->door_v;
	if (r->is_facing_left)
		r->offset = (int)((r->o + game->doors[d->door_index].door_prog)
				* img->w);
	else if (r->is_facing_right)
		r->offset = (int)((r->o + game->doors[d->door_index].door_prog)
				* img->w);
	return (img);
}
