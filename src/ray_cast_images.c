/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_images.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:17:53 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:37:49 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static t_image	*facing_up(t_game *game, t_r *r, t_image *img)
{
	if (img == NULL)
	{
		img = &game->pack.no_img;
		r->offset = (int)(r->o * img->w);
	}
	return (img);
}

static t_image	*facing_down(t_game *game, t_r *r, t_image *img)
{
	if (img == NULL)
	{
		img = &game->pack.so_img;
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	return (img);
}

static t_image	*facing_right(t_game *game, t_r *r, t_image *img)
{
	if (img == NULL)
	{
		img = &game->pack.ea_img;
		r->offset = (int)(r->o * img->w);
	}
	return (img);
}

static t_image	*facing_left(t_game *game, t_r *r, t_image *img)
{
	if (img == NULL)
	{
		img = &game->pack.we_img;
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
