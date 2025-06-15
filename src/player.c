/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:27:03 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 19:38:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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

	if (!map || !*map)
		return (0);
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

void	get_w_and_h(char **map, int *w, int *h)
{
	*h = ft_array_lenght(map);
	*w = ft_strlen(map[0]);
}

double	get_player_angle(char **map, t_player player)
{
	if (map[(int)player.pos.y][(int)player.pos.x] == p_n)
		return (1.5 * PI);
	else if (map[(int)player.pos.y][(int)player.pos.x] == p_s)
		return (0.5 * PI);
	else if (map[(int)player.pos.y][(int)player.pos.x] == p_e)
		return (0);
	else if (map[(int)player.pos.y][(int)player.pos.x] == p_w)
		return (PI);
	return (361);
}

void	init_player(t_game *game)
{
	game->txt.p = get_player_cell(game->map);
	get_axis(game->map, game->txt.p, &game->player.pos.x, &game->player.pos.y);
	game->player.angle = get_player_angle(game->map, game->player);
	game->player.key.key_up = false;
	game->player.key.key_down = false;
	game->player.key.key_right = false;
	game->player.key.key_left = false;
	game->player.key.rot_l = false;
	game->player.key.rot_r = false;
	game->player.turn_dir = 0;
	game->player.walk_dir = 0;
	game->player.pos.x += 0.5;
	game->player.pos.y += 0.5;
	get_w_and_h(game->map, &game->player.w, &game->player.h);
}
