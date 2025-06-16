/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_extra_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:48:42 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/16 17:59:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

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
				game->player.key.key_jump = false;
			}
		}
	}
}
