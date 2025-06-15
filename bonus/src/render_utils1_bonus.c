/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils1_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 00:54:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:47:35 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

int	get_img_color(t_image *img, int x, int y)
{
	return (*(int *)(img->addr + ((y * img->size_line) + (x * (img->bpp
					/ 8)))));
}

void	put_pixel(t_image *img, int x, int y, int color)
{
	int	offset;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	offset = (y * img->size_line) + (x * img->bpp / 8);
	img->addr[offset] = color & 0xFF;
	img->addr[offset + 1] = (color >> 8) & 0xFF;
	img->addr[offset + 2] = (color >> 16) & 0xFF;
}

int	set_color_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}
