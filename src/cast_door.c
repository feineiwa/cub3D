/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:56:20 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/10 14:37:27 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

t_image	*change_dist_for_door(t_game *game, t_image *img, t_r *r)
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
	r->o = set_hit_and_dist_wall(r);
	img = check_if_hit_door(game, r, img);
	return (img);
}

void	middle_door_north_and_south(t_r *r, t_d *d)
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

void	middle_door_east_and_west(t_r *r, t_d *d)
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

t_image	*slide_door(t_game *game, t_r *r, t_d *d, t_image *img)
{
	if (r->horizontal_dist < r->vertical_dist)
	{
		slide_door_north_and_south(r, d);
		img = get_door_image_north_and_south(game, r, d, img);
	}
	else
	{
		slide_door_east_and_west(r, d);
		img = get_door_image_east_and_west(game, r, d, img);
	}
	return (img);
}

t_image	*cast_ray_door(t_game *game, t_r *r)
{
	t_image	*img;
	t_d		d;

	img = NULL;
	d.door_index = find_door(game->doors, (int)floor(r->wall_hit.x / TILE_SIZE),
			(int)floor(r->wall_hit.y / TILE_SIZE));
	set_door_in_middle(r, &d);
	if (r->o <= game->doors[d.door_index].door_prog)
		img = change_dist_for_door(game, img, r);
	else
		img = slide_door(game, r, &d, img);
	return (img);
}
