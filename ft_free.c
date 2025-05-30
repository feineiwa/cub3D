/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:24:45 by frahenin          #+#    #+#             */
/*   Updated: 2025/05/30 14:41:52 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	ft_free_image(t_game *game, t_image image)
{
	if (image.img_ptr != NULL)
		mlx_destroy_image(game->var.mlx, image.img_ptr);
}

void	ft_free_img_loop(t_game *game, t_image *img, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		ft_free_image(game, img[i]);
		i++;
	}
}

void	ft_free_img_packs(t_game *game, t_pack packs)
{
	ft_free_image(game, packs.arrow);
	ft_free_image(game, packs.aiming);
	ft_free_image(game, packs.no_img);
	ft_free_image(game, packs.so_img);
	ft_free_image(game, packs.ea_img);
	ft_free_image(game, packs.we_img);
	ft_free_image(game, packs.floor);
	ft_free_image(game, packs.ceil);
	ft_free_image(game, packs.compass);
	ft_free_image(game, packs.needle);
	ft_free_image(game, packs.scope);
	ft_free_image(game, packs.blood);
	ft_free_image(game, packs.gov);
	ft_free_image(game, packs.notice);
	ft_free_image(game, packs.help);
	ft_free_img_loop(game, packs.gun_sp, 15);
	ft_free_img_loop(game, packs.door, 2);
	ft_free_img_loop(game, packs.walk, 4);
	ft_free_img_loop(game, packs.death, 7);
	ft_free_image(game, packs.pain);
	ft_free_image(game, packs.load);
}

void	free_all(t_game *game)
{	
	ft_free_image(game, game->img);
	ft_free_img_packs(game , game->pack);
	ft_free(game->sp);
	ft_free(game->doors);
	ft_free_img_loop(game, game->intro.img, 384);
	mlx_clear_window(game->var.mlx, game->var.win);
	if (game->var.win != NULL)
		mlx_destroy_window(game->var.mlx, game->var.win);
	if (game->var.mlx != NULL)
	{
		mlx_destroy_display(game->var.mlx);
		ft_free(game->var.mlx);
	}
	ft_free_txt(&game->txt);
	ft_free_arr((void **)game->map);
}
