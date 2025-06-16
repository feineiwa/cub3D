/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   video_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:55:57 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/16 17:07:28 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

static void	set_filename(char *filename, int j)
{
	if (j >= 10 && j <= 99)
	{
		filename[35] = '0' + (j / 10);
		filename[36] = '0' + (j % 10);
	}
	else if (j > 99)
	{
		filename[34] = '0' + ((j / 10) / 10);
		filename[35] = '0' + ((j / 10) % 10);
		filename[36] = '0' + (j % 10);
	}
	else
		filename[36] = '0' + j;
}

void	load_video(t_game *game)
{
	char	filename[42];
	int		i;
	int		j;

	ft_memcpy((char *)filename,
		(char *)"/home/frahenin/goinfre/video/intro000.xpm", 42);
	i = 0;
	game->intro.frm = 0;
	game->intro.last_time = get_current_time();
	while (i < 384)
	{
		j = i + 1;
		set_filename(filename, j);
		game->intro.img[i].img_ptr = mlx_xpm_file_to_image(game->var.mlx,
				filename, &game->intro.img[i].w, &game->intro.img[i].h);
		if (game->intro.img[i].img_ptr != NULL)
			game->intro.img[i].addr = mlx_get_data_addr(\
					game->intro.img[i].img_ptr,
					&game->intro.img[i].bpp, &game->intro.img[i].size_line,
					&game->intro.img[i].endian);
		i++;
	}
}

int	video_running(t_game *game)
{
	if (g_global()->first == true && game->intro.img[game->intro.frm].img_ptr)
	{
		mlx_put_image_to_window(game->var.mlx, game->var.win,
			game->intro.img[game->intro.frm].img_ptr, 0, 0);
		if (get_current_time() - game->intro.last_time >= 90
			&& game->intro.frm < 383)
		{
			game->intro.frm++;
			game->intro.last_time = get_current_time();
		}
		return (0);
	}
	return (1);
}
