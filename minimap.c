/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:23:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/05 08:23:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	draw_minimap(t_image *img, char **map, t_player player, t_game *game)
{
	int	x1;
	int	x2;
	int	y1;
	int	y2;
	int	side = 7;
	double angle;
	
	angle = normalize_angle(player.angle + (0.5 * PI));
	x1 = (int)floor(player.pos.x) - side;
	if (x1 < 0)
		x1 = 0;
	x2 = player.pos.x + side;
	if (x2 > player.w)
		x2 = player.w;
	y1 = (int)floor(player.pos.y) - side;
	if (y1 < 0)
		y1 = 0;
	y2 = player.pos.y + side;
	if (y2 > player.h)
		y2 = player.h;
	t_vector c = {100, HEIGHT - 100};
	draw_circle_full(img, c, MINI_WIDTH / 2, 0x1E1E1E);
	draw_circle(img, c, MINI_WIDTH / 2, 0x333333);
	draw_circle(img, c, MINI_WIDTH / 2 + 1, 0x555555);
	draw_circle(img, c, MINI_WIDTH / 2 + 2, 0x777777);
	draw_circle(img, c, MINI_WIDTH / 2 + 3, 0x999999);
	double cos_angle = cos(angle);
    double sin_angle = sin(angle);
	int	j = y1;
	int	i;
	while (j < y2)
	{
		i = x1;
		while (i < x2)
		{
			t_vector relative = {(i - player.pos.x), (j - player.pos.y)};
			t_vector rotated = {relative.x * cos_angle + relative.y * sin_angle, -relative.x * sin_angle + relative.y * cos_angle};
			t_vect draw = {(int)(c.x + (rotated.x * MINI_SIZE)), (int)(c.y + (rotated.y * MINI_SIZE))};
			if (map[j][i] == wall)
				draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0xE0E0E0);
			else if (map[j][i] == door_c)
				draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0x00FFFF);
			else if (map[j][i] == door_o)
				draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0x007070);
				// draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0xFF0000);
			// draw_square_at_edge(img, draw.x, draw.y, MINI_SIZE, c, 0xE0E0E0);
			
			i++;
		}
		j++;
	}
	int	sp_c = 0;
	while (sp_c < g_global()->sp_count)
	{
		t_vector relative = {(game->sp[sp_c].pos.x - player.pos.x), (game->sp[sp_c].pos.y - player.pos.y)};
		t_vector rotated = {relative.x * cos_angle + relative.y * sin_angle, -relative.x * sin_angle + relative.y * cos_angle};
		t_vect draw = {(int)(c.x + (rotated.x * MINI_SIZE)), (int)(c.y + (rotated.y * MINI_SIZE))};
		if (game->sp[sp_c].is_died == false)
			draw_circle_enemy(img, (t_vector){draw.x, draw.y}, 3, 0xFF0000, c);
		else
			draw_circle_enemy(img, (t_vector){draw.x, draw.y}, 3, 0x4D1F1F, c);
		sp_c++;
	}
	// draw_circle(img, c, 1, 0x00FF66);
	// draw_square(img, (int)c.x, (int)c.y, 5, 0x00FF66);
}