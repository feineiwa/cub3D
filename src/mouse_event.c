/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_event.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:21:39 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/09 17:30:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	mouse_key_pressed(int button, t_game *game)
{
	if (button == 1)
	{
		// if (!is_music_playing())
		// 	launch_music("sounds/gun1.wav", "0.5");
		// g_global()->one_shoot_music = true;
		game->player.key.recharge = true;
		game->player.key.shoot = true;
		g_global()->updated = true;
	}
	if (button == 4)
	{
		game->player.key.scope = true;
		g_global()->zoom += 0.1;
		g_global()->narrow_look += 100;
		g_global()->updated = true;
	}
	if (button == 5)
	{
		g_global()->zoom -= 0.1;
		g_global()->narrow_look -= 100;
		g_global()->updated = true;
	}
}

int	ft_mouse_pressed(int button, int x, int y, t_game *game)
{
	(void)x;
	(void)y;
	if (game->player.key.hide_mouse == false)
		return (0);
	mouse_key_pressed(button, game);
	if (button == 3)
	{
		game->player.key.scope = false;
		g_global()->zoom = 1.0;
		g_global()->narrow_look = 875;
		g_global()->updated = true;
	}
	if (g_global()->zoom < 1)
	{
		game->player.key.scope = false;
		g_global()->zoom = 1;
		g_global()->narrow_look = 875;
	}
	if (g_global()->zoom > 3.0)
	{
		g_global()->zoom = 3.0;
		g_global()->narrow_look = 2875;
	}
	return (0);
}

int	ft_mouse_release(int button, int x, int y, t_game *game)
{
	(void)x;
	(void)y;
	if (game->player.key.hide_mouse == false)
		return (0);
	if (button == 1)
		game->player.key.shoot = false;
	return (0);
}

int	ft_mouse_move(int x, int y, t_game *game)
{
	int	dx;
	int	dy;

	if (game->player.key.hide_mouse == false)
		return (1);
	dx = x - HALF_WIDTH;
	dy = y - HALF_HEIGHT;
	if (dx != 0 || dy != 0)
	{
		game->player.angle += dx * 1e-4;
		g_global()->look -= dy * 1e-4;
		if (g_global()->look <= -0.3)
			g_global()->look = -0.3;
		else if (g_global()->look >= 0.3)
			g_global()->look = 0.3;
		g_global()->updated = true;
		mlx_mouse_move(game->var.mlx, game->var.win, HALF_WIDTH, HALF_HEIGHT);
	}
	return (0);
}
