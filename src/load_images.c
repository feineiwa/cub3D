/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_images.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:27:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 14:44:52 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	load_image(t_game *game, t_image *img, char *path)
{
	img->img_ptr = mlx_xpm_file_to_image(game->var.mlx, path, &img->w, &img->h);
	if (img->img_ptr == NULL)
	{
		free_all(game);
		ft_putendl_fd("Error", 2);
		ft_putstr_fd("Failed to load image. Please ", 2);
		ft_putendl_fd("check the file path, format, and permissions.", 2);
		exit(1);
	}
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp,
			&img->size_line, &img->endian);
}

void	load_all_images(t_game *game)
{
	load_image(game, &game->pack.no_img, game->txt.no_path);
	load_image(game, &game->pack.so_img, game->txt.so_path);
	load_image(game, &game->pack.ea_img, game->txt.ea_path);
	load_image(game, &game->pack.we_img, game->txt.we_path);
}
