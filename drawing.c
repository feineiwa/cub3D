/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:55:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/03 17:59:19 by frahenin         ###   ########.fr       */
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
	int	half;
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
