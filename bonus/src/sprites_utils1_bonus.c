/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_utils1_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 23:28:42 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:47:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

static t_sprit	init_sprite_data(int x, int y)
{
	t_sprit	sp;

	sp.pos.x = x + 0.5;
	sp.pos.y = y + 0.5;
	sp.w_frm = 0;
	sp.w_fps = get_current_time();
	sp.health = 3;
	sp.d_fps = get_current_time();
	sp.d_frm = 0;
	sp.was_shot = false;
	sp.is_died = false;
	return (sp);
}

t_sprit	*init_sprite(char **map)
{
	int		x;
	int		y;
	t_sprit	*sp;
	int		k;

	sp = malloc(sizeof(t_sprit) * g_global()->sp_count);
	if (!sp)
		return (NULL);
	y = 0;
	k = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == sprite)
			{
				sp[k] = init_sprite_data(x, y);
				k++;
			}
			x++;
		}
		y++;
	}
	return (sp);
}

int	count_sprite(char **map)
{
	int	i;
	int	j;
	int	count;

	j = 0;
	count = 0;
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == sprite)
				count++;
			i++;
		}
		j++;
	}
	return (count);
}

void	attack_to_player(t_game *game, int sp_c)
{
	float	dist_for_die;

	dist_for_die = distance_between_points(game->sp[sp_c].pos,
			game->player.pos);
	if (dist_for_die < 0.5 && !game->sp[sp_c].is_died)
	{
		game->player.health--;
		paint_image_to_win(&game->img, &game->pack.blood, (t_vect){WIDTH,
			HEIGHT}, (t_vect){0, 0});
		if (game->player.health <= 0)
			g_global()->game_over = true;
	}
}

void	move_sprite(t_game *game, int sp_c)
{
	t_vector	move;

	if (game->sp[sp_c].is_died == false && game->sp[sp_c].was_shot == false)
	{
		move = (t_vector){game->sp[sp_c].pos.x - cos(game->sp[sp_c].p) * 0.02,
			game->sp[sp_c].pos.y - sin(game->sp[sp_c].p) * 0.02};
		if (!check_collision(move, game->player, game->map))
		{
			game->sp[sp_c].pos.x = move.x;
			game->sp[sp_c].pos.y = move.y;
		}
		else
		{
			if (!check_collision((t_vector){move.x, game->sp[sp_c].pos.y},
				game->player, game->map))
				game->sp[sp_c].pos.x = move.x;
			if (!check_collision((t_vector){game->sp[sp_c].pos.x, move.y},
				game->player, game->map))
				game->sp[sp_c].pos.y = move.y;
		}
		attack_to_player(game, sp_c);
	}
}
