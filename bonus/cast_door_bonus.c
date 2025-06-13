/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_door_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:56:20 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:47:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

static t_image	*slide_door(t_game *game, t_r *r, t_d *d, t_image *img)
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

static t_image	*change_dist_for_door(t_game *game, t_image *img, t_r *r)
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
