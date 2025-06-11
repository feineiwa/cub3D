/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:16:53 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/11 23:53:50 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	key_of_move(int keysym, t_game *game)
{
	if (keysym == XK_w)
	{
		game->player.key.key_up = true;
		game->player.walk_dir = 1;
	}
	if (keysym == XK_s)
	{
		game->player.key.key_down = true;
		game->player.walk_dir = -1;
	}
	if (keysym == XK_d)
	{
		game->player.key.key_right = true;
		game->player.walk_dir_side = 1;
	}
	if (keysym == XK_a)
	{
		game->player.key.key_left = true;
		game->player.walk_dir_side = -1;
	}
	if (keysym == XK_Shift_L)
		game->player.key.speed = true;
}

void	key_of_look(int keysym, t_game *game)
{
	if (keysym == XK_Left)
	{
		game->player.key.rot_l = true;
		game->player.turn_dir = -1;
	}
	if (keysym == XK_Right)
	{
		game->player.key.rot_r = true;
		game->player.turn_dir = 1;
	}
	if (keysym == XK_Up)
		game->player.key.look_up = true;
	if (keysym == XK_Down)
		game->player.key.look_down = true;
}

void	key_for_extra(int keysym, t_game *game)
{
	if (keysym == XK_Alt_L)
	{
		if (game->player.key.hide_mouse == false)
		{
			game->player.key.hide_mouse = true;
			mlx_mouse_hide(game->var.mlx, game->var.win);
			mlx_mouse_move(game->var.mlx, game->var.win, HALF_WIDTH,
				HALF_HEIGHT);
		}
		else
		{
			game->player.key.hide_mouse = false;
			mlx_mouse_show(game->var.mlx, game->var.win);
		}
	}
	if (keysym == XK_c)
		game->player.key.key_croutch = true;
	if (keysym == XK_space && game->player.key.key_jump == false)
		game->player.key.key_jump = true;
	if (keysym == XK_f && game->player.key.door_check == false)
	{
		game->player.key.door_check = true;
		// if (!is_music_playing())
		// 	launch_music("./sounds/sliding.mp3", "0.5");
	}
}

int	ft_key_press(int keysym, t_game *game)
{
	key_of_move(keysym, game);
	key_of_look(keysym, game);
	if (keysym == XK_Return)
	{
		if (g_global()->first == true)
			stop_music();
		g_global()->first = false;
	}
	if (game->bonus)
		key_for_extra(keysym, game);
	if (keysym == XK_F1)
	{
		if (g_global()->help == false)
			g_global()->help = true;
		else
			g_global()->help = false;
	}
	if (keysym == XK_Escape)
	{
		free_all(game);
		stop_music();
		exit(0);
	}
	g_global()->updated = true;
	return (0);
}

int	ft_key_release(int keysym, t_game *game)
{
	if (keysym == XK_w)
		game->player.key.key_up = false;
	if (keysym == XK_s)
		game->player.key.key_down = false;
	if (keysym == XK_d)
		game->player.key.key_right = false;
	if (keysym == XK_a)
		game->player.key.key_left = false;
	if (keysym == XK_Left)
		game->player.key.rot_l = false;
	if (keysym == XK_Right)
		game->player.key.rot_r = false;
	if (keysym == XK_Shift_L)
		game->player.key.speed = false;
	if (keysym == XK_Up)
		game->player.key.look_up = false;
	if (keysym == XK_Down)
		game->player.key.look_down = false;
	if (keysym == XK_c)
		game->player.key.key_croutch = false;
	return (0);
}
