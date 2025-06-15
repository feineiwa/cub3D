/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:24:45 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 19:32:00 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	ft_free_img_packs(t_game *game, t_pack packs)
{
	ft_free_image(game, packs.no_img);
	ft_free_image(game, packs.so_img);
	ft_free_image(game, packs.ea_img);
	ft_free_image(game, packs.we_img);
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
	free_game_var(game);
}

int	leave_windows(t_game *game)
{
	free_all(game);
	exit(0);
}
