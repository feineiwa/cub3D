/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:06:40 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 00:38:15 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

bool	is_hit_door(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == door_c || map[y1][x1] == door_o)
		return (true);
	return (false);
}

bool	is_hit_door_close(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == door_c)
		return (true);
	return (false);
}

bool	is_hit_door_open(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == door_o)
		return (true);
	return (false);
}

t_image	*check_if_hit_door(t_game *game, t_r *r, t_image *img)
{
	if (is_hit_door((int)floor(r->wall_hit.x), (int)floor(r->wall_hit.y),
			game->map, game->player))
	{
		img = cast_ray_door(game, r);
		img = get_image_by_side(game, r, img);
	}
	return (img);
}
