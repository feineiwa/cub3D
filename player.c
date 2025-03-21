/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:27:03 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/21 15:55:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	get_axis(char **map, char tile, double *x, double *y)
{
	int	i;
	int	j;

	j = 0;
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == tile)
			{
				*x = i;
				*y = j;
				return ;
			}
			i++;
		}
		j++;
	}
}

t_cell	get_player_cell(char **map)
{
	int	i;
	int	j;

	j = 0;
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == p_n || map[j][i] == p_s || map[j][i] == p_e
				|| map[j][i] == p_w)
				return (map[j][i]);
			i++;
		}
		j++;
	}
	return (0);
}

void	init_player(t_game *game)
{
	game->txt.p = get_player_cell(game->map);
	get_axis(game->map, game->txt.p, &game->player.pos.x, &game->player.pos.y);
	if (game->map[(int)game->player.pos.y][(int)game->player.pos.x] == p_n)
	{
		game->player.dir.y = -1;
		game->player.dir.x = 0;
		game->player.plan.x = 0.66;
		game->player.plan.y = 0;
		// game->angle = MYPI;
	}
	else if (game->map[(int)game->player.pos.y][(int)game->player.pos.x] == p_s)
	{
		game->player.dir.y = 1;
		game->player.dir.x = 0;
		game->player.plan.x = -0.66;
		game->player.plan.y = 0;
		// game->angle = MYPI / 2;
	}
	else if (game->map[(int)game->player.pos.y][(int)game->player.pos.x] == p_e)
	{
		game->player.dir.y = 0;
        game->player.dir.x = 1;
        game->player.plan.x = 0;
        game->player.plan.y = 0.66;
		// game->angle = 0;
	}
	else if (game->map[(int)game->player.pos.y][(int)game->player.pos.x] == p_w)
	{
		game->player.dir.y = 0;
        game->player.dir.x = -1;
        game->player.plan.x = 0;
        game->player.plan.y = -0.66;
		// game->angle = (3 * MYPI) / 2;
	}
}
