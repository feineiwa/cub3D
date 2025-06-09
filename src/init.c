/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:15:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/09 17:30:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	init_window(t_game *game)
{
	game->var.mlx = mlx_init();
	if (!game->var.mlx)
		return (0);
	game->var.win = mlx_new_window(game->var.mlx, WIDTH, HEIGHT, "Cub3d");
	if (!game->var.win)
	{
		mlx_destroy_display(game->var.mlx);
		ft_free(game->var.mlx);
		return (0);
	}
	return (1);
}

int	init_image_win(t_game *game, t_image *image, int w, int h)
{
	image->img_ptr = mlx_new_image(game->var.mlx, w, h);
	if (!image->img_ptr)
		return (0);
	image->addr = mlx_get_data_addr(image->img_ptr, &image->bpp,
			&image->size_line, &image->endian);
	return (1);
}

t_sprit	init_sprite_data(int x, int y)
{
	t_sprit	sp;

	sp.pos.x = x + 0.5;
	sp.pos.y = y + 0.5;
	sp.w_frm = 0;
	sp.w_fps = get_current_time();
	sp.health = 3;
	sp.d_fps = get_current_time();
	sp.d_frm = 0;
	sp.was_shot = false;
	sp.is_died = false;
	return (sp);
}

t_sprit	*init_sprite(char **map)
{
	int		x;
	int		y;
	t_sprit	*sp;
	int		k;

	sp = malloc(sizeof(t_sprit) * g_global()->sp_count);
	if (!sp)
		return (NULL);
	y = 0;
	k = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == sprite)
			{
				sp[k] = init_sprite_data(x, y);
				k++;
			}
			x++;
		}
		y++;
	}
	return (sp);
}

void	init_game(t_game *game)
{
	game->map = NULL;
	game->var.mlx = NULL;
	game->var.win = NULL;
	game->sp = NULL;
	game->doors = NULL;
	game->img.img_ptr = NULL;
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
	game->pack.aiming.img_ptr = NULL;
	game->pack.arrow.img_ptr = NULL;
	game->pack.no_img.img_ptr = NULL;
	game->pack.so_img.img_ptr = NULL;
	game->pack.ea_img.img_ptr = NULL;
	game->pack.we_img.img_ptr = NULL;
	game->pack.floor.img_ptr = NULL;
	game->pack.ceil.img_ptr = NULL;
	game->pack.compass.img_ptr = NULL;
	game->pack.needle.img_ptr = NULL;
	game->pack.scope.img_ptr = NULL;
	game->pack.blood.img_ptr = NULL;
	game->pack.gov.img_ptr = NULL;
	game->pack.notice.img_ptr = NULL;
	game->pack.help.img_ptr = NULL;
	game->pack.pain.img_ptr = NULL;
	game->pack.load.img_ptr = NULL;
	init_img_loop(game->pack.gun_sp, 15);
	init_img_loop(game->pack.door, 2);
	init_img_loop(game->pack.walk, 4);
	init_img_loop(game->pack.death, 7);
	init_img_loop(game->intro.img, 384);
}
