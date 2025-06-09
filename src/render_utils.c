/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 08:13:26 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/09 17:30:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	put_pixel(t_image *img, int x, int y, int color)
{
	int	offset;

	// char	*offset;
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	offset = (y * img->size_line) + (x * img->bpp / 8);
	img->addr[offset] = color & 0xFF;
	img->addr[offset + 1] = (color >> 8) & 0xFF;
	img->addr[offset + 2] = (color >> 16) & 0xFF;
	// offset = img->img_ptr + (y * img->size_line + x * (img->bpp / 8));
	// *(int *)offset = color;
}

int	set_color_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

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

int	get_img_color(t_image *img, int x, int y)
{
	return (*(int *)(img->addr + ((y * img->size_line) + (x * (img->bpp
						/ 8)))));
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
