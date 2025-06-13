/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 01:07:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:22:12 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
