/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:23:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 00:43:52 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static t_vect	clamp(t_vect point, t_player player)
{
	if (point.x < 0)
		point.x = 0;
	if (point.x > player.w)
		point.x = player.w;
	if (point.y < 0)
		point.y = 0;
	if (point.y > player.h)
		point.y = player.h;
	return (point);
}

static void	get_draw_point(t_squ *squ, t_vector c)
{
	squ->rotated = (t_vector){squ->relative.x * cos(squ->angle)
		+ squ->relative.y * sin(squ->angle), -squ->relative.x * sin(squ->angle)
		+ squ->relative.y * cos(squ->angle)};
	squ->draw = (t_vect){(int)(c.x + (squ->rotated.x * MINI_SIZE)), (int)(c.y
			+ (squ->rotated.y * MINI_SIZE))};
}

static void	draw_grid(t_game *game, t_vector c, t_vect s1, t_vect s2)
{
	int		i;
	t_squ	sq;

	sq.angle = normalize_angle(game->player.angle + (0.5 * PI));
	sq.c = c;
	while (s1.y < s2.y)
	{
		i = s1.x - 1;
		while (++i < s2.x)
		{
			sq.relative = (t_vector){(i - game->player.pos.x), (s1.y
					- game->player.pos.y)};
			get_draw_point(&sq, c);
			if (game->map[s1.y][i] == wall)
				draw_rotated_square(&game->img, sq, MINI_SIZE - 1, 0xE0E0E0);
			else if (game->map[s1.y][i] == door_c)
				draw_rotated_square(&game->img, sq, MINI_SIZE - 1, 0x00FFFF);
			else if (game->map[s1.y][i] == door_o)
				draw_rotated_square(&game->img, sq, MINI_SIZE - 1, 0x007070);
		}
		s1.y++;
	}
}

static void	draw_enemy_rotated(t_game *game, t_image *img, t_vector c,
		t_player player)
{
	int		sp_c;
	t_squ	squ;

	sp_c = 0;
	squ.angle = normalize_angle(player.angle + (0.5 * PI));
	squ.c = c;
	while (sp_c < g_global()->sp_count)
	{
		squ.relative = (t_vector){(game->sp[sp_c].pos.x - player.pos.x),
			(game->sp[sp_c].pos.y - player.pos.y)};
		get_draw_point(&squ, c);
		if (game->sp[sp_c].is_died == false)
			draw_circle_enemy(img, squ, 3, 0xFF0000);
		else
			draw_circle_enemy(img, squ, 3, 0x4D1F1F);
		sp_c++;
	}
}

void	draw_minimap(t_image *img, t_player player, t_game *game)
{
	int			side;
	t_vect		s1;
	t_vect		s2;
	t_vector	c;

	side = 7;
	s1.x = (int)floor(player.pos.x) - side;
	s2.x = (int)floor(player.pos.x) + side;
	s1.y = (int)floor(player.pos.y) - side;
	s2.y = (int)floor(player.pos.y) + side;
	s1 = clamp(s1, game->player);
	s2 = clamp(s2, game->player);
	c = (t_vector){100, HEIGHT - 100};
	draw_circle_full(img, c, MINI_WIDTH / 2, 0x1E1E1E);
	draw_circle(img, c, MINI_WIDTH / 2, 0x333333);
	draw_circle(img, c, MINI_WIDTH / 2 + 1, 0x555555);
	draw_circle(img, c, MINI_WIDTH / 2 + 2, 0x777777);
	draw_circle(img, c, MINI_WIDTH / 2 + 3, 0x999999);
	draw_grid(game, c, s1, s2);
	draw_enemy_rotated(game, img, c, player);
}
