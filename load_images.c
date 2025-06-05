/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_images.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:27:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/05 13:51:08 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	load_image(t_game *game, t_image *img, char *path)
{
	img->img_ptr = mlx_xpm_file_to_image(game->var.mlx, path, &img->w, &img->h);
	if (img->img_ptr == NULL)
	{
		free_all(game);
		ft_putendl_fd("Error\nFailed to load image. Please check the file path, format, and permissions.", 2);
		exit(1);
	}
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->size_line, &img->endian);
}

void	load_image_loop(t_game *game, t_image *img, const char *base_path, int limit)
{
	int	i;
	int	index;
	int	j;
	char file[256];

	i = 0;
	index = -1;
	ft_memmove((char *)file, (char *)base_path, ft_strlen(base_path));
	if (limit == 15)
		index = 10;
	else if (limit == 4)
		index = 15;
	else if (limit == 7)
		index = 18;
	else if (limit == 384)
		index = 13;
	while (i < limit)
	{
		j = i + 1;
		if (j >= 10 && j <= 99)
		{
			file[index + 1] = '0' + (j / 10);
			file[index + 2] = '0' + (j % 10);
		}
		else if (j > 99)
		{
			file[index] = '0' + ((j / 10) / 10);
			file[index + 1] = '0' + ((j / 10) % 10);
			file[index + 2] = '0' + (j % 10);
		}
		else
			file[index + 2] = '0' + j;
		printf("%s\n", file);
		load_image(game, &img[i], file);
		i++;
	}
}
void	load_pack(t_game *game)
{
	load_image(game, &game->pack.arrow, "images/arrow.xpm");
	load_image(game, &game->pack.aiming, "images/aiming1.xpm");
	load_image(game, &game->pack.floor, "images/floor6.xpm");
	load_image(game, &game->pack.ceil, "images/ceil0.xpm");
	load_image(game, &game->pack.door[0], "images/my_door/door.xpm");
	load_image(game, &game->pack.door[1], "images/my_door/door_side2.xpm");
	load_image(game, &game->pack.compass, "images/compass2.xpm");
	load_image(game, &game->pack.needle, "images/needle.xpm");
	load_image(game, &game->pack.scope, "images/scope.xpm");
	load_image(game, &game->pack.blood, "images/blood.xpm");
	load_image(game, &game->pack.gov, "images/gov.xpm");
	load_image(game, &game->pack.help, "images/help.xpm");
	load_image(game, &game->pack.notice, "images/notice.xpm");
	load_image(game, &game->pack.walk[0], "images/Sprites/sp1.xpm");
	load_image(game, &game->pack.walk[1], "images/Sprites/sp2.xpm");
	load_image(game, &game->pack.walk[2], "images/Sprites/sp3.xpm");
	load_image(game, &game->pack.walk[3], "images/Sprites/sp4.xpm");
	load_image(game, &game->pack.pain, "images/Sprites/pain1.xpm");
	load_image(game, &game->pack.death[0], "images/Sprites/death1.xpm");
	load_image(game, &game->pack.death[1], "images/Sprites/death2.xpm");
	load_image(game, &game->pack.death[2], "images/Sprites/death3.xpm");
	load_image(game, &game->pack.death[3], "images/Sprites/death4.xpm");
	load_image(game, &game->pack.death[4], "images/Sprites/death5.xpm");
	load_image(game, &game->pack.death[5], "images/Sprites/death6.xpm");
	load_image(game, &game->pack.death[6], "images/Sprites/death7.xpm");
}

void	load_gun_sp(t_game *game)
{
	char	file[18];
	int		i;

	ft_memmove((void *)file, (const void *)"images/gun/00.xpm", 18);
	i = 0;
	while (i < 15)
	{
		if (i <= 8)
			file[12] = (i + 1) + '0';
		else
		{
			file[12] = ((i + 1) % 10) + '0';
			file[11] = ((i + 1) / 10) + '0';
		}
		load_image(game, &game->pack.gun_sp[i], file);
		i++;
	}
}

void	load_all_images(t_game *game)
{
	load_image(game, &game->pack.no_img, game->txt.no_path);
	load_image(game, &game->pack.so_img, game->txt.so_path);
	load_image(game, &game->pack.ea_img, game->txt.ea_path);
	load_image(game, &game->pack.we_img, game->txt.we_path);
	load_image(game, &game->pack.load, "images/loading.xpm");
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->pack.load.img_ptr, 0, 0);
	load_pack(game);
	load_gun_sp(game);
	load_video(game);
}

