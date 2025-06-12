/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_door_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 01:01:59 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 01:04:03 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	middle_door_north_and_south(t_r *r, t_d *d)
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
	if (d->door_h_dist < d->door_v_dist)
		r->o = ((fmod(d->door_h.x, TILE_SIZE) / TILE_SIZE));
	else
		r->o = ((fmod(d->door_v.y, TILE_SIZE) / TILE_SIZE));
}

static void	middle_door_east_and_west(t_r *r, t_d *d)
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
	if (d->door_h_dist < d->door_v_dist)
		r->o = ((fmod(d->door_h.x, TILE_SIZE) / TILE_SIZE));
	else
		r->o = 1.0 - ((fmod(d->door_v.y, TILE_SIZE) / TILE_SIZE));
}

void	set_door_in_middle(t_r *r, t_d *d)
{
	if (r->horizontal_dist < r->vertical_dist)
		middle_door_north_and_south(r, d);
	else
		middle_door_east_and_west(r, d);
}
