/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utils2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 01:11:11 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:47:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

void	draw_circle_enemy(t_image *img, t_squ squ, int radius, int color)
{
	int		i;
	int		j;
	int		dist_squared;
	double	dist;

	j = -radius;
	while (j <= radius)
	{
		i = -radius;
		while (i <= radius)
		{
			dist_squared = (i * i) + (j * j);
			if (dist_squared <= (radius * radius))
			{
				dist = (squ.draw.x + i - squ.c.x) * (squ.draw.x + i - squ.c.x)
					+ (squ.draw.y + j - squ.c.y) * (squ.draw.y + j - squ.c.y);
				if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
					put_pixel(img, squ.draw.x + i, squ.draw.y + j, color);
			}
			i++;
		}
		j++;
	}
}

void	draw_texture(t_image *img, t_image *txt, t_r *r)
{
	int		i;
	int		color;
	double	tex_pos;
	t_squ	s;

	tex_pos = 0;
	s.start = (t_vect){(int)r->ray * RES, (int)r->draw_begin};
	s.end = (t_vect){(int)RES, (int)r->draw_end};
	s.scale = (double)txt->h / (double)(s.end.y - s.start.y);
	while (s.start.y <= s.end.y)
	{
		i = -1;
		s.tex.y = (int)tex_pos;
		if (s.tex.y >= txt->h)
			s.tex.y = txt->h - 1;
		while (++i < s.end.x)
		{
			color = get_img_color(txt, r->offset, s.tex.y);
			if (color >= 0)
				put_pixel(img, s.start.x, s.start.y, color_by_distance(color,
						r->dist_wall, 60));
		}
		tex_pos += s.scale;
		s.start.y++;
	}
}
