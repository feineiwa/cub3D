/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils2_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:13:26 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:47:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

void	clear_image(t_image *img, int width, int height, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	paint_image_to_win_aiming(t_image *img, t_image *img1, t_vect s,
		t_vect p)
{
	int	x;
	int	y;
	int	img_color;
	int	src_x;
	int	src_y;

	y = 0;
	while (y < s.y)
	{
		x = 0;
		while (x < s.x)
		{
			src_x = (int)(double)x * ((double)img1->w / (double)s.x);
			src_y = (int)(double)y * ((double)img1->h / (double)s.y);
			img_color = get_img_color(img1, src_x, src_y);
			if (img_color >= 0)
			{
				img_color = 0xFF0000;
				put_pixel(img, x + p.x, y + p.y, img_color);
			}
			x++;
		}
		y++;
	}
}

void	paint_image_to_win_arrow(t_image *img, t_image *img1, t_vect s,
		t_vect p)
{
	int	x;
	int	y;
	int	img_color;
	int	src_x;
	int	src_y;

	y = 0;
	while (y < s.y)
	{
		x = 0;
		while (x < s.x)
		{
			src_x = (int)(double)x * ((double)img1->w / (double)s.x);
			src_y = (int)(double)y * ((double)img1->h / (double)s.y);
			img_color = get_img_color(img1, src_x, src_y);
			if (img_color >= 0)
			{
				img_color = 0x00FF00;
				put_pixel(img, x + p.x, y + p.y, img_color);
			}
			x++;
		}
		y++;
	}
}

void	paint_image_to_win(t_image *img, t_image *img1, t_vect s, t_vect p)
{
	int	x;
	int	y;
	int	img_color;
	int	src_x;
	int	src_y;

	y = 0;
	while (y < s.y)
	{
		x = 0;
		while (x < s.x)
		{
			src_x = (int)(double)x * ((double)img1->w / (double)s.x);
			src_y = (int)(double)y * ((double)img1->h / (double)s.y);
			img_color = get_img_color(img1, src_x, src_y);
			if (img_color >= 0)
				put_pixel(img, x + p.x, y + p.y, img_color);
			x++;
		}
		y++;
	}
}

int	get_health_color(int health, int max_health)
{
	t_rgb	color;
	float	ratio;

	ratio = (float)health / max_health;
	color.r = (int)((1.0f - ratio) * 255);
	color.g = (int)(ratio * 255);
	color.b = 0;
	return (set_color_rgb(color.r, color.g, color.b));
}
