/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:23:53 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/10 17:51:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	move_up_and_down(t_game *game, double move_speed)
{
	t_vector	move;

	if (game->player.key.key_up == true || game->player.key.key_down == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle)
			* game->player.walk_dir * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle)
			* game->player.walk_dir * move_speed;
		if (!check_collision(move, game->player, game->map))
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
		else
		{
			if (!check_collision((t_vector){move.x, game->player.pos.y},
				game->player, game->map))
				game->player.pos.x = move.x;
			if (!check_collision((t_vector){game->player.pos.x, move.y},
				game->player, game->map))
				game->player.pos.y = move.y;
		}
	}
}

void	move_right_and_left(t_game *game, double move_speed)
{
	t_vector	move;

	if (game->player.key.key_right == true || game->player.key.key_left == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle + 0.5 * PI)
			* game->player.walk_dir_side * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle + 0.5 * PI)
			* game->player.walk_dir_side * move_speed;
		if (!is_exceeded(move, game->player) && !is_collision(move, game->map))
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
		else
		{
			if (!check_collision((t_vector){move.x, game->player.pos.y},
				game->player, game->map))
				game->player.pos.x = move.x;
			if (!check_collision((t_vector){game->player.pos.x, move.y},
				game->player, game->map))
				game->player.pos.y = move.y;
		}
	}
}

void	look_up_and_down(t_game *game)
{
	if (game->player.key.look_up == true)
	{
		g_global()->look += (float)(ROT_SPEED);
		if (g_global()->look >= 0.3)
			g_global()->look = 0.3;
	}
	if (game->player.key.look_down == true)
	{
		g_global()->look -= (float)(ROT_SPEED);
		if (g_global()->look <= -0.3)
			g_global()->look = -0.3;
	}
}

void	crouch_player(t_game *game)
{
	if (game->player.key.key_croutch == true)
	{
		if (g_global()->j_croutch == 0)
		{
			g_global()->j_croutch = -300;
			g_global()->look += 0.2;
		}
		else
		{
			g_global()->j_croutch = 0;
			g_global()->look = 0;
			game->player.key.key_croutch = false;
		}
	}
}

void	jump_player(t_game *game)
{
	if (game->player.key.key_jump == true)
	{
		if (g_global()->jump > 0 && g_global()->j_croutch < 500)
		{
			g_global()->j_croutch += g_global()->jump;
			g_global()->look -= 0.08;
			if (g_global()->j_croutch >= 500)
				g_global()->jump *= -1;
		}
		else if (g_global()->jump < 0)
		{
			g_global()->j_croutch += g_global()->jump;
			g_global()->look += 0.08;
			if (g_global()->j_croutch <= 0)
			{
				g_global()->j_croutch = 0;
				g_global()->jump = 100;
				g_global()->updated = false;
				game->player.key.key_jump = false;
			}
		}
	}
}

void	move_player(t_game *game)
{
	double	move_speed;

	move_speed = MOVE_SPEED;
	if (game->player.key.speed == true)
		move_speed += MOVE_SPEED;
	if (game->player.key.rot_l == true)
		game->player.angle += game->player.turn_dir * move_speed;
	if (game->player.key.rot_r == true)
		game->player.angle += game->player.turn_dir * move_speed;
	move_up_and_down(game, move_speed);
	move_right_and_left(game, move_speed);
	look_up_and_down(game);
	crouch_player(game);
	jump_player(game);
	game->player.key.rot_l = false;
	game->player.key.rot_r = false;
	game->player.key.look_up = false;
	game->player.key.look_down = false;
}
