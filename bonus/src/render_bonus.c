/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:30:14 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 19:55:13 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

int	game_over(t_game *game)
{
	if (g_global()->game_over == true)
	{
		paint_image_to_win(&game->img, &game->pack.gov, (t_vect){WIDTH, HEIGHT},
			(t_vect){0, 0});
		mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr,
			0, 0);
		return (0);
	}
	return (1);
}

void	render_ceil_and_floor(t_game *game, t_r *r)
{
	draw_floor_and_ceil(game, &game->pack.ceil, r, false);
	draw_floor_and_ceil(game, &game->pack.floor, r, true);
}

void	render_gun_by_frame(t_game *game, size_t *last_update, t_vect g)
{
	if (g_global()->gun_frm < 15)
	{
		paint_image_to_win(&game->img, &game->pack.gun_sp[g_global()->gun_frm],
			(t_vect){500, 500}, g);
		if (get_current_time() - *last_update >= FPS)
		{
			g_global()->gun_frm++;
			*last_update = get_current_time();
		}
	}
	else
	{
		game->player.key.recharge = false;
		g_global()->gun_frm = 0;
		*last_update = 0;
		g_global()->updated = true;
	}
}

void	render_gun(t_game *game, t_r *r)
{
	static size_t	last_update = 0;
	t_vect			g;

	g.x = 800 - 100;
	g.y = (int)(HEIGHT - 300 - (g_global()->look * 50) - (r->waves * 10));
	if (game->player.key.recharge == true)
	{
		g_global()->updated = true;
		render_gun_by_frame(game, &last_update, g);
	}
	else if (game->player.key.key_jump == false
		&& game->player.key.scope == false)
	{
		paint_image_to_win(&game->img, &game->pack.gun_sp[0], (t_vect){500,
			500}, g);
		g_global()->updated = false;
	}
}

void	draw_compass(t_game *game)
{
	t_squ	s;

	s.angle = game->player.angle;
	s.c = (t_vector){WIDTH - 121, 95};
	paint_image_to_win(&game->img, &game->pack.compass, (t_vect){150, 150},
		(t_vect){WIDTH - 200, 22});
	draw_rotated_texture(&game->img, &game->pack.needle, s, (t_vect){100, 100});
}
