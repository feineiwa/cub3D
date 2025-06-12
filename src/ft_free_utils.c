/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 23:42:22 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/12 23:42:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	ft_free_image(t_game *game, t_image image)
{
	if (image.img_ptr != NULL)
		mlx_destroy_image(game->var.mlx, image.img_ptr);
}
