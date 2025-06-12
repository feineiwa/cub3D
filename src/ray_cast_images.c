/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_images.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:17:53 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 00:20:27 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static t_image	*facing_up(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.no_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE));
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE) + 1;
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = (int)(r->o * img->w);
	}
	return (img);
}

static t_image	*facing_down(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.so_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE));
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE) - 1;
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	return (img);
}

static t_image	*facing_right(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.ea_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE) - 1);
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE);
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = (int)(r->o * img->w);
	}
	return (img);
}

static t_image	*facing_left(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.we_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE) + 1);
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE);
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	return (img);
}

t_image	*get_image_by_side(t_game *game, t_r *r, t_image *img)
{
	if (r->is_facing_up && r->horizontal_dist < r->vertical_dist)
		img = facing_up(game, r, img);
	else if (r->is_facing_down && r->horizontal_dist < r->vertical_dist)
		img = facing_down(game, r, img);
	else if (r->is_facing_right && r->horizontal_dist > r->vertical_dist)
		img = facing_right(game, r, img);
	else if (r->is_facing_left && r->horizontal_dist > r->vertical_dist)
		img = facing_left(game, r, img);
	return (img);
}
