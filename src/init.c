/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:15:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/17 13:25:42 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	init_image_win(t_game *game, t_image *image, int w, int h)
{
	image->img_ptr = mlx_new_image(game->var.mlx, w, h);
	if (!image->img_ptr)
		return (0);
	image->addr = mlx_get_data_addr(image->img_ptr, &image->bpp,
			&image->size_line, &image->endian);
	return (1);
}

void	init_img_loop(t_image *img, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		img[i].img_ptr = NULL;
		i++;
	}
}

void	init_all_images(t_game *game)
{
	game->pack.no_img.img_ptr = NULL;
	game->pack.so_img.img_ptr = NULL;
	game->pack.ea_img.img_ptr = NULL;
	game->pack.we_img.img_ptr = NULL;
}

int	init_window(t_game *game)
{
	game->var.mlx = mlx_init();
	if (!game->var.mlx)
		return (0);
	game->var.win = mlx_new_window(game->var.mlx, WIDTH, HEIGHT, "Cub3D");
	if (!game->var.win)
	{
		mlx_destroy_display(game->var.mlx);
		ft_free(game->var.mlx);
		return (0);
	}
	return (1);
}

void	init_game(t_game *game)
{
	game->map = NULL;
	game->var.mlx = NULL;
	game->var.win = NULL;
	game->img.img_ptr = NULL;
}
