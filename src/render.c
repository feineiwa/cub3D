/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:30:14 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:43:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	render_ceil_and_floor(t_game *game, t_r *r)
{
	draw_rectangle(&game->img, (t_vect){r->ray, 0}, (t_vect){r->ray + RES,
		(int)r->draw_begin}, set_color_rgb(game->txt.c.r, game->txt.c.g,
			game->txt.c.b));
	draw_rectangle(&game->img, (t_vect){r->ray, (int)r->draw_end},
		(t_vect){r->ray + RES, HEIGHT}, set_color_rgb(game->txt.f.r,
			game->txt.f.g, game->txt.f.b));
}
