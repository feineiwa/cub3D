/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 01:07:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 01:09:45 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
