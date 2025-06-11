/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:55:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/12 00:09:40 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	draw_circle(t_image *img, t_vector p, int radius, int color)
{
	int	i;
	int	j;
	int	dist_squared;

	j = -radius;
	while (j <= radius)
	{
		i = -radius;
		while (i <= radius)
		{
			dist_squared = (i * i) + (j * j);
			if ((dist_squared >= ((radius * radius) - radius))
				&& (dist_squared <= ((radius * radius) + radius)))
				put_pixel(img, p.x + i, p.y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_rectangle(t_image *image, t_vect a, t_vect b, int color)
{
	int	i;

	while (a.y < b.y)
	{
		i = 0;
		while (i < b.x - a.x)
		{
			put_pixel(image, i + a.x, a.y, color);
			i++;
		}
		a.y++;
	}
}

void	draw_circle_full(t_image *img, t_vector p, int radius, int color)
{
	int	i;
	int	j;
	int	dist_squared;

	j = -radius;
	while (j <= radius)
	{
		i = -radius;
		while (i <= radius)
		{
			dist_squared = (i * i) + (j * j);
			if (dist_squared <= (radius * radius))
				put_pixel(img, p.x + i, p.y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_rotated_square(t_image *img, t_squ squ, int size, int color)
{
	int			x;
	int			y;
	t_vect		draw;
	t_vector	rot;
	double		dist;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			rot.x = x * cos(squ.angle) + y * sin(squ.angle);
			rot.y = -x * sin(squ.angle) + y * cos(squ.angle);
			draw.x = (int)(squ.draw.x + rot.x);
			draw.y = (int)(squ.draw.y + rot.y);
			dist = (draw.x - squ.c.x) * (draw.x - squ.c.x) + (draw.y - squ.c.y)
				* (draw.y - squ.c.y);
			if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
				put_pixel(img, draw.x, draw.y, color);
			x++;
		}
		y++;
	}
}

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

int	color_by_distance(int color, double distance, int light_level)
{
	t_rgb	col;
	double	light;

	light = light_level / distance;
	col.r = (color >> 16) & 0xFF;
	col.g = (color >> 8) & 0xFF;
	col.b = color & 0xFF;
	col.r = fmin(fmax(col.r * light, 0), col.r);
	col.g = fmin(fmax(col.g * light, 0), col.g);
	col.b = fmin(fmax(col.b * light, 0), col.b);
	return (set_color_rgb(col.r, col.g, col.b));
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

void	set_data_f(t_game *game, t_r *r, t_floor *f, bool fl)
{
	f->tile_size_inv = 1.0 / TILE_SIZE;
	f->cos_ray = cos(r->ray_angle);
	f->sin_ray = sin(r->ray_angle);
	f->beta = normalize_angle(r->ray_angle - game->player.angle);
	if (fl)
	{
		f->start = r->draw_end + 1;
		f->end = HEIGHT;
	}
	else
	{
		f->start = 0;
		f->end = r->draw_begin;
	}
}

void	draw_floor_and_ceil(t_game *game, t_image *texture, t_r *r, bool fl)
{
	t_floor	f;

	set_data_f(game, r, &f, fl);
	while (f.start < f.end)
	{
		if (fl)
			f.diff = f.start - r->middle;
		else
			f.diff = r->middle - f.start;
		f.s = (TILE_SIZE * g_global()->project_dist) / f.diff;
		f.distance = f.s / fabs(cos(f.beta) + 1.0);
		f.f.x = r->a.x + f.cos_ray * f.distance;
		f.f.y = r->a.y + f.sin_ray * f.distance;
		f.tex_x = abs((int)((fmod(f.f.x, TILE_SIZE) * \
			f.tile_size_inv) * texture->w));
		f.tex_y = abs((int)((fmod(f.f.y, TILE_SIZE) * \
			f.tile_size_inv) * texture->h));
		f.color = get_img_color(texture, f.tex_x, f.tex_y);
		put_pixel(&game->img, r->ray, f.start, color_by_distance(f.color,
				f.distance, 60));
		f.start++;
	}
}

void	draw_rotated_texture(t_image *dest, t_image *src, t_squ s, t_vect size)
{
	int			y;
	int			x;
	int			color;

	s.cos_a = cos(s.angle + (0.5 * PI));
	s.sin_a = sin(s.angle + (0.5 * PI));
	y = -1;
	while (++y < size.y)
	{
		x = -1;
		while (++x < size.y)
		{
			s.tex.x = (int)(double)x * ((double)src->w / (double)size.x);
			s.tex.y = (int)(double)y * ((double)src->h / (double)size.y);
			color = get_img_color(src, s.tex.x, s.tex.y);
			if (color >= 0)
			{
				s.d = (t_vect){x - (size.x / 2), y - (size.y / 2)};
				s.r.x = s.cos_a * s.d.x - s.sin_a * s.d.y;
				s.r.y = s.sin_a * s.d.x + s.cos_a * s.d.y;
				put_pixel(dest, (int)((s.c.x + s.r.x)), (int)(s.c.y + s.r.y),
					color);
			}
		}
	}
}

void	adjusted_sprite(t_sprit sp, t_squ *s, float n)
{
	s->start.y = (int)(sp.sp_screen.y - ((int)sp.proj_sp_height / 2) * n);
	s->end.y = s->start.y + (int)sp.proj_sp_height;
	s->start.x = (int)(sp.sp_screen.x - (int)sp.proj_sp_height / 2);
	s->end.x = s->start.x + (int)sp.proj_sp_height;
}

void	draw_sprite(t_game *game, t_image *img, t_r r, t_sprit sp)
{
	t_squ	s;
	t_vect	i;
	int		color;

	adjusted_sprite(sp, &s, 0.4);
	i.x = s.start.x - 1;
	while (++i.x < s.end.x)
	{
		if (i.x >= 0 && i.x < WIDTH && sp.dist < r.ray_dist[i.x])
		{
			s.tex.x = (i.x - s.start.x) * img->w / (int)sp.proj_sp_height;
			i.y = s.start.y - 1;
			while (++i.y < s.end.y && i.y >= 0 && i.y < HEIGHT)
			{
				s.tex.y = (i.y - s.start.y) * img->h / (int)sp.proj_sp_height;
				color = get_img_color(img, s.tex.x, s.tex.y);
				if (color >= 0)
					put_pixel(&game->img, i.x, i.y, color_by_distance(color,
							sp.dist, 60));
			}
		}
	}
}
