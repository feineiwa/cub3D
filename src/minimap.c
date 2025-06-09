/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:23:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/09 23:34:32 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

t_vect	clamp(t_vect point, t_player player)
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

void	draw_grid(t_game *game, t_vector c, t_vect s1, t_vect s2)
{
	int			i;
	double		angle;
	t_vector	rotated;
	t_vector	relative;
	t_vect		draw;

	angle = normalize_angle(game->player.angle + (0.5 * PI));
	while (s1.y < s2.y)
	{
		i = s1.x;
		while (i < s2.x)
		{
			relative = (t_vector){(i - game->player.pos.x), (s1.y - game->player.pos.y)};
			rotated = (t_vector){relative.x * cos(angle) + relative.y * sin(angle), -relative.x * sin(angle) + relative.y * cos(angle)};
			draw = (t_vect){(int)(c.x + (rotated.x * MINI_SIZE)), (int)(c.y + (rotated.y * MINI_SIZE))};
			if (game->map[s1.y][i] == wall)
				draw_rotated_square(&game->img, draw, c, MINI_SIZE - 1, angle, 0xE0E0E0);
			else if (game->map[s1.y][i] == door_c)
				draw_rotated_square(&game->img, draw, c, MINI_SIZE - 1, angle, 0x00FFFF);
			else if (game->map[s1.y][i] == door_o)
				draw_rotated_square(&game->img, draw, c, MINI_SIZE - 1, angle, 0x007070);
			i++;
		}
		s1.y++;
	}
}

void	draw_door_grid(t_game *game, t_image *img, t_vector c, t_player player)
{
	int			sp_c;
	double		angle;
	t_vector	relative;
	t_vector	rotated;
	t_vect		draw;

	sp_c = 0;
	angle = normalize_angle(player.angle + (0.5 * PI));
	while (sp_c < g_global()->sp_count)
	{
		relative = (t_vector){(game->sp[sp_c].pos.x - player.pos.x), (game->sp[sp_c].pos.y - player.pos.y)};
		rotated = (t_vector){relative.x * cos(angle) + relative.y * sin(angle), -relative.x * sin(angle) + relative.y * cos(angle)};
		draw = (t_vect){(int)(c.x + (rotated.x * MINI_SIZE)), (int)(c.y + (rotated.y * MINI_SIZE))};
		if (game->sp[sp_c].is_died == false)
			draw_circle_enemy(img, (t_vector){draw.x, draw.y}, 3, 0xFF0000, c);
		else
			draw_circle_enemy(img, (t_vector){draw.x, draw.y}, 3, 0x4D1F1F, c);
		sp_c++;
	}
}

void	draw_minimap(t_image *img, char **map, t_player player, t_game *game)
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
	draw_door_grid(game, img, c, player);
	(void)map;
	// draw_circle(img, c, 1, 0x00FF66);
	// draw_square(img, (int)c.x, (int)c.y, 5, 0x00FF66);
}
