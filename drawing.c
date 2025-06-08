/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:55:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/07 10:49:37 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	draw_line(t_image *img, t_vector a, t_vector b, int color)
{
	double	dx;
	double	dy;
	double	steps;
	double	xinc;
	double	yinc;
	int		i;

	dx = (b.x - a.x);
	dy = (b.y - a.y);
	steps = fmax(fabs(dx), fabs(dy));
	xinc = dx / steps;
	yinc = dy / steps;
	i = 0;
	while (i <= steps)
	{
		a.x += xinc;
		a.y += yinc;
		put_pixel(img, a.x, a.y, color);
		i++;
	}
}

void	draw_square(t_image *img, int x, int y, int size, int color)
{
	int	j;
	int	i;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size)
		{
			put_pixel(img, i + x, j + y, color);
			i++;
		}
		j++;
	}
}

void	draw_circle(t_image *img, t_vector p, int radius, int color)
{
	int	i;
	int	j;
	int	dist_squared;

	j = -radius;
	// put_pixel(img, p.x, p.y, color);
	while (j <= radius)
	{
		i = -radius;
		while (i <= radius)
		{
			dist_squared = (i * i) + (j * j);
			if ((dist_squared >= ((radius * radius) - radius)) && (dist_squared <= ((radius * radius) + radius)))
				put_pixel(img, p.x + i, p.y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_rectangle_rgb_gradient(t_image *image, int	x0, int x1, int y0, int y1, t_rgb rgb)
{
	int	i;
	int	round;

	round = 0;
	while (y0 < y1)
	{
		i = 0;
		while (i < x1)
		{
			put_pixel(image, i + x0, y0, set_color_rgb(rgb.r, rgb.g, rgb.b));
			i++;
		}
		round++;
		if (round == 1)
		{
			rgb.r -= 1;
			rgb.g -= 1;
			rgb.b -= 1;
			if (rgb.r < 0)
				rgb.r = 0;
			if (rgb.g < 0)
				rgb.g = 0;
			if (rgb.b < 0)
				rgb.b = 0;
			round = 0;
		}
		y0++;
	}
}

void	draw_rectangle_rgb_rgradient(t_image *image, int x0, int x1, int y0, int y1, t_rgb rgb)
{
	int	i;
	int	round;

	round = 0;
	while (y1 > y0)
	{
		i = 0;
		while (i < x1)
		{
			put_pixel(image, i + x0, y1, set_color_rgb(rgb.r, rgb.g, rgb.b));
			i++;
		}
		round++;
		if (round == 3)
		{
			rgb.r -= 1;
			rgb.g -= 1;
			rgb.b -= 1;
			if (rgb.r < 0)
				rgb.r = 0;
			if (rgb.g < 0)
				rgb.g = 0;
			if (rgb.b < 0)
				rgb.b = 0;
			round = 0;
		}
		y1--;
	}
}

void	draw_rectangle(t_image *image, int	x0, int x1, int y0, int y1, int color)
{
	int	i;

	while (y0 < y1)
	{
		i = x0;
		while (i < x1)
		{
			put_pixel(image, i, y0, color);
			i++;
		}
		y0++;
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
			if ( dist_squared <= (radius * radius))
				put_pixel(img, p.x + i, p.y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_square_at_edge(t_image *img, int x, int y, int size, t_vector c, int color)
{
	int	j;
	int	i;
	int	px;
	int	py;
	double	dist;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size)
		{
			px = i + x;
			py = j + y;
			dist = (px - c.x) * (px - c.x) + (py - c.y) * (py - c.y);
			if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
				put_pixel(img, px, py, color);
			i++;
		}
		j++;
	}
}

void	draw_rotated_square(t_image *img, t_vect s_p, t_vector c, int size, double angle, int color)
{
	int	x;
	int	y;
	t_vect draw;
	t_vector rot;
	double dist;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			rot.x = x * cos(angle) + y * sin(angle);
			rot.y = -x * sin(angle) + y * cos(angle);
			draw.x = (int)(s_p.x + rot.x);
			draw.y = (int)(s_p.y + rot.y);
			dist = (draw.x - c.x) * (draw.x - c.x) + (draw.y - c.y) * (draw.y - c.y);
			if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
				put_pixel(img, draw.x, draw.y, color);
			x++;
		}
		y++;
	}
}

void	draw_circle_enemy(t_image *img, t_vector p, int radius, int color, t_vector c)
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
			if ( dist_squared <= (radius * radius))
			{
				double dist = (p.x + i - c.x) * (p.x + i - c.x) + (p.y + j - c.y) * (p.y + j - c.y);

				if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
					put_pixel(img, p.x + i, p.y + j, color);
			}
			i++;
		}
		j++;
	}
}

void	draw_texture(t_image *img, t_image *txt, int x0, int x1, int y0, int y1, int offset, double dist_wall)
{
	int	i;
	int color;
	double scale;
	int	texture_y;
	double	light = 40 / dist_wall;
	t_rgb col;
	int	r;
	int	g;
	int	b;
	double	tex_pos = 0;

	scale = (double)txt->h / (double)(y1 - y0);
	while (y0 <= y1)
	{
		i = 0;
		texture_y = (int)tex_pos;
		if (texture_y >= txt->h)
			texture_y = txt->h - 1;
		while (i < x1)
		{
			color = get_img_color(txt, offset, texture_y);
			if (color >= 0)
			{
				r = (color >> 16) & 0xFF;
				g = (color >> 8) & 0xFF;
				b = color & 0xFF;
				// col.r = r;
				// col.g = g;
				// col.b = b;
				col.r = fmin(fmax(r * light, 0), r);
				col.g = fmin(fmax(g * light, 0), g);
				col.b = fmin(fmax(b * light, 0), b);
				put_pixel(img, i + x0, y0, set_color_rgb(col.r, col.g, col.b));
			}
			i++;
		}
		tex_pos += scale;
		y0++;	
	}
}

void	draw_floor_and_ceil(t_game *game, t_image *texture, int ray, double ray_angle, int start, int end, double middle, bool fl)
{
	t_vector p;
	int		color;
	double	cos_ray;
	double	sin_ray;
	double	distance;
	double r;
	double s;
	
	p.x = (double)game->player.pos.x * TILE_SIZE;
	p.y = (double)game->player.pos.y * TILE_SIZE;
	double	tile_size_inv = 1.0 / TILE_SIZE;
	cos_ray = cos(ray_angle);
	sin_ray = sin(ray_angle);
	double	beta = normalize_angle(ray_angle - game->player.angle);
	while (start < end)
	{
		if (fl)
			r = start - middle;
		else
			r = middle - start;
		s = (TILE_SIZE * g_global()->project_dist) / r;
		distance = s / fabs(cos(beta) + 1.0);
		t_vector f;
		f.x = p.x + cos_ray * distance;
		f.y = p.y + sin_ray * distance;
		int tex_x = (int)((fmod(f.x, TILE_SIZE) * tile_size_inv) * texture->w);
		int tex_y = (int)((fmod(f.y, TILE_SIZE) * tile_size_inv) * texture->h);
		tex_x = abs(tex_x); 
        tex_y = abs(tex_y);
		color = get_img_color(texture, tex_x, tex_y);
		put_pixel(&game->img, ray, start, color);
		start++;
	}
}

void	draw_rotated_texture(t_image *dest, t_image *src, t_vector center, double angle, int s_x, int s_y)
{
	int		y;
	int		x;
	int		color;
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle + (0.5 * PI));
	sin_a = sin(angle + (0.5 * PI));
	y = 0;
	while (y < s_y)
	{
		x = 0;
		while (x < s_y)
		{
			int src_x = (int)(double)x * ((double)src->w / (double)s_x);
			int src_y = (int)(double)y * ((double)src->h / (double)s_y);
			color = get_img_color(src, src_x, src_y);
			if (color >= 0)
			{
				t_vect d = {
					.x = x - (s_x / 2),
					.y = y - (s_y / 2)
				};
				t_vector r = {
					.x = cos_a * d.x - sin_a * d.y,
					.y = sin_a * d.x + cos_a * d.y
				};
				put_pixel(dest, (int)((center.x + r.x)), (int)(center.y + r.y), color);
			}
			x++;
		}
		y++;
	}
}

void	draw_sprite(t_game *game, t_vector sp_screen, double proj_sp_height, t_image *img, double ray_dist[WIDTH], double dist)
{
	int	sprite_height = (int)proj_sp_height;
	int	sprite_width = sprite_height;
	int	start_y = (int)(sp_screen.y - (sprite_height / 2) * 0.4);
	int end_y = start_y + sprite_height;
	int	start_x = (int)(sp_screen.x - sprite_width / 2);
	int	end_x = start_x + sprite_width;
	int	x = start_x;
	int	y = start_y;
	while (x < end_x)
	{
		if (x >= 0 && x < WIDTH && dist < ray_dist[x])
		{
			int	tex_x = (x - start_x) * img->w / sprite_width;
			y = start_y;
			while (y < end_y)
			{
				if (y >= 0 && y < HEIGHT)
				{
					int tex_y = (y - start_y) * img->h / sprite_height;
					int	color = get_img_color(img, tex_x, tex_y);
					if (color >= 0)
						put_pixel(&game->img, x, y, color);
				}
				y++;
			}
		}
		x++;
	}
}