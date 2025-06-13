/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 01:11:11 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:32:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
	while (s.start.y < s.end.y)
	{
		i = -1;
		s.tex.y = (int)tex_pos;
		if (s.tex.y >= txt->h)
			s.tex.y = txt->h - 1;
		while (++i < s.end.x)
		{
			color = get_img_color(txt, r->offset, s.tex.y);
			if (color >= 0)
				put_pixel(img, s.start.x + i, s.start.y, color);
		}
		tex_pos += s.scale;
		s.start.y++;
	}
}
