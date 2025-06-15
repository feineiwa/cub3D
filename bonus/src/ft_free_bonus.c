/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:24:45 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 19:52:13 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

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
	ft_free_image(game, packs.no_img);
	ft_free_image(game, packs.so_img);
	ft_free_image(game, packs.ea_img);
	ft_free_image(game, packs.we_img);
	ft_free_image(game, packs.arrow);
	ft_free_image(game, packs.aiming);
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

void	free_game_var(t_game *game)
{
	ft_free_image(game, game->img);
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

void	free_all(t_game *game)
{
	ft_free_img_packs(game, game->pack);
	ft_free(game->sp);
	ft_free(game->doors);
	ft_free_img_loop(game, game->intro.img, 384);
	free_game_var(game);
}

int	leave_windows(t_game *game)
{
	free_all(game);
	stop_music();
	exit(0);
}
