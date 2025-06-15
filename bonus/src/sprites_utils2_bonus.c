/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_utils2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 23:23:18 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:47:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

void	render_sprite_pain(t_game *game, t_r r, int sp_c)
{
	draw_sprite(game, &game->pack.pain, r, game->sp[sp_c]);
	if (game->sp[sp_c].was_shot == false)
	{
		game->sp[sp_c].health--;
		game->sp[sp_c].was_shot = true;
		if (game->sp[sp_c].health <= 0 && game->sp[sp_c].was_shot)
		{
			game->sp[sp_c].is_died = true;
			game->sp[sp_c].d_fps = get_current_time();
			game->sp[sp_c].d_frm = 0;
		}
	}
}

void	render_sprite_died(t_game *game, t_r r, int sp_c)
{
	if (game->sp[sp_c].d_frm <= 6 && get_current_time()
		- game->sp[sp_c].d_fps <= 10000)
		draw_sprite(game, &game->pack.death[game->sp[sp_c].d_frm], r,
			game->sp[sp_c]);
	if (get_current_time() - game->sp[sp_c].d_fps >= 100)
	{
		if (game->sp[sp_c].d_frm < 6)
		{
			game->sp[sp_c].d_frm++;
			game->sp[sp_c].d_fps = get_current_time();
		}
	}
	game->sp[sp_c].was_shot = false;
}

void	render_sprite_walk(t_game *game, t_r r, int sp_c)
{
	draw_sprite(game, &game->pack.walk[game->sp[sp_c].w_frm], r,
		game->sp[sp_c]);
	if (get_current_time() - game->sp[sp_c].w_fps >= 300)
	{
		game->sp[sp_c].w_frm++;
		game->sp[sp_c].w_fps = get_current_time();
	}
	if (game->sp[sp_c].w_frm > 3)
		game->sp[sp_c].w_frm = 0;
	game->sp[sp_c].was_shot = false;
}
