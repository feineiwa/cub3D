/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:16:53 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 14:45:57 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	key_of_move(int keysym, t_game *game)
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
	if (keysym == XK_Shift_L && game->bonus)
		game->player.key.speed = true;
}

static void	key_of_look(int keysym, t_game *game)
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
	if (game->bonus)
	{
		if (keysym == XK_Up)
			game->player.key.look_up = true;
		if (keysym == XK_Down)
			game->player.key.look_down = true;
	}
}

int	ft_key_press(int keysym, t_game *game)
{
	key_of_move(keysym, game);
	key_of_look(keysym, game);
	if (keysym == XK_Escape)
	{
		free_all(game);
		exit(0);
	}
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
	return (0);
}
