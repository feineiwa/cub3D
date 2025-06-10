/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:55:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/10 17:05:28 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

// void	draw_line(t_image *img, t_vector a, t_vector b, int color)
// {
// 	double	dx;
// 	double	dy;
// 	double	steps;
// 	double	xinc;
// 	double	yinc;
// 	int		i;

// 	dx = (b.x - a.x);
// 	dy = (b.y - a.y);
// 	steps = fmax(fabs(dx), fabs(dy));
// 	xinc = dx / steps;
// 	yinc = dy / steps;
// 	i = 0;
// 	while (i <= steps)
// 	{
// 		a.x += xinc;
// 		a.y += yinc;
// 		put_pixel(img, a.x, a.y, color);
// 		i++;
// 	}
// }

// void	draw_square(t_image *img, int x, int y, int size, int color)
// {
// 	int	j;
// 	int	i;

// 	j = 0;
// 	while (j < size)
// 	{
// 		i = 0;
// 		while (i < size)
// 		{
// 			put_pixel(img, i + x, j + y, color);
// 			i++;
// 		}
// 		j++;
// 	}
// }

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
		i = a.x;
		while (i < b.x)
		{
			put_pixel(image, i, a.y, color);
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

// void	draw_square_at_edge(t_image *img, int x, int y, int size, t_vector c,
// 		int color)
// {
// 	int		j;
// 	int		i;
// 	int		px;
// 	int		py;
// 	double	dist;

// 	j = 0;
// 	while (j < size)
// 	{
// 		i = 0;
// 		while (i < size)
// 		{
// 			px = i + x;
// 			py = j + y;
// 			dist = (px - c.x) * (px - c.x) + (py - c.y) * (py - c.y);
// 			if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
// 				put_pixel(img, px, py, color);
// 			i++;
// 		}
// 		j++;
// 	}
// }

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
	double	scale;
	int		texture_y;
	double	tex_pos;
	t_vect	a;
	t_vect	b;

	tex_pos = 0;
	a = (t_vect){(int)r->ray * RES, (int)r->draw_begin};
	b = (t_vect){(int)RES, (int)r->draw_end};
	scale = (double)txt->h / (double)(b.y - a.y);
	while (a.y <= b.y)
	{
		i = -1;
		texture_y = (int)tex_pos;
		if (texture_y >= txt->h)
			texture_y = txt->h - 1;
		while (++i < b.x)
		{
			color = get_img_color(txt, r->offset, texture_y);
			if (color >= 0)
				put_pixel(img, a.x, a.y, color_by_distance(color,
						r->dist_wall, 60));
		}
		tex_pos += scale;
		a.y++;
	}
}

void	draw_floor_and_ceil(t_game *game, t_image *texture, int ray,
		double ray_angle, int start, int end, double middle, bool fl)
{
	t_vector	p;
	int			color;
	double		cos_ray;
	double		sin_ray;
	double		distance;
	double		r;
	double		s;
	int			tex_x;
	int			tex_y;
	double		tile_size_inv;
	double		beta;
	t_vector	f;

	p.x = (double)game->player.pos.x * TILE_SIZE;
	p.y = (double)game->player.pos.y * TILE_SIZE;
	tile_size_inv = 1.0 / TILE_SIZE;
	cos_ray = cos(ray_angle);
	sin_ray = sin(ray_angle);
	beta = normalize_angle(ray_angle - game->player.angle);
	while (start < end)
	{
		if (fl)
			r = start - middle;
		else
			r = middle - start;
		s = (TILE_SIZE * g_global()->project_dist) / r;
		distance = s / fabs(cos(beta) + 1.0);
		f.x = p.x + cos_ray * distance;
		f.y = p.y + sin_ray * distance;
		tex_x = (int)((fmod(f.x, TILE_SIZE) * tile_size_inv) * texture->w);
		tex_y = (int)((fmod(f.y, TILE_SIZE) * tile_size_inv) * texture->h);
		tex_x = abs(tex_x);
		tex_y = abs(tex_y);
		color = get_img_color(texture, tex_x, tex_y);
		put_pixel(&game->img, ray, start, color_by_distance(color, distance,
				60));
		start++;
	}
}

void	draw_rotated_texture(t_image *dest, t_image *src, t_vector center,
		double angle, int s_x, int s_y)
{
	int			y;
	int			x;
	int			color;
	double		cos_a;
	double		sin_a;
	t_vect		d;
	t_vector	r;
	int			src_x;
	int			src_y;

	cos_a = cos(angle + (0.5 * PI));
	sin_a = sin(angle + (0.5 * PI));
	y = 0;
	while (y < s_y)
	{
		x = 0;
		while (x < s_y)
		{
			src_x = (int)(double)x * ((double)src->w / (double)s_x);
			src_y = (int)(double)y * ((double)src->h / (double)s_y);
			color = get_img_color(src, src_x, src_y);
			if (color >= 0)
			{
				d = (t_vect){x - (s_x / 2), y - (s_y / 2)};
				r = (t_vector){cos_a * d.x - sin_a * d.y, sin_a * d.x + cos_a
					* d.y};
				put_pixel(dest, (int)((center.x + r.x)), (int)(center.y + r.y),
					color);
			}
			x++;
		}
		y++;
	}
}

void	draw_sprite(t_game *game, t_vector sp_screen, double proj_sp_height,
		t_image *img, double ray_dist[WIDTH], double dist)
{
	int	sprite_height;
	int	sprite_width;
	int	start_y;
	int	end_y;
	int	start_x;
	int	end_x;
	int	x;
	int	y;
	int	tex_x;
	int	tex_y;
	int	color;

	sprite_height = (int)proj_sp_height;
	sprite_width = sprite_height;
	start_y = (int)(sp_screen.y - (sprite_height / 2) * 0.4);
	end_y = start_y + sprite_height;
	start_x = (int)(sp_screen.x - sprite_width / 2);
	end_x = start_x + sprite_width;
	x = start_x;
	y = start_y;
	while (x < end_x)
	{
		if (x >= 0 && x < WIDTH && dist < ray_dist[x])
		{
			tex_x = (x - start_x) * img->w / sprite_width;
			y = start_y;
			while (y < end_y)
			{
				if (y >= 0 && y < HEIGHT)
				{
					tex_y = (y - start_y) * img->h / sprite_height;
					color = get_img_color(img, tex_x, tex_y);
					if (color >= 0)
						put_pixel(&game->img, x, y, color_by_distance(color,
								dist, 60));
				}
				y++;
			}
		}
		x++;
	}
}
