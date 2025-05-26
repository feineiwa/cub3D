/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   video.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:55:57 by frahenin          #+#    #+#             */
/*   Updated: 2025/05/26 18:10:05 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	load_video(t_game *game)
{
	char	filename[21];
	int		i;
	int		j;

	ft_memcpy((char *)filename, (char *)"./video/intro000.xpm", 21);
	i = 0;
	game->intro.frm = 0;
	game->intro.last_time = get_current_time();
	while (i < 172)
	{
		j = i + 1;
		if (j >= 10 && j <= 99)
		{
			filename[14] = '0' + (j / 10);
			filename[15] = '0' + (j % 10);
		}
		else if (j > 99)
		{
			filename[13] = '0' + ((j / 10) / 10);
			filename[14] = '0' + ((j / 10) % 10);
			filename[15] = '0' + (j % 10);
		}
		else
			filename[15] = '0' + j;
		// fprintf(stderr, "%s\n", filename);
		game->intro.img[i].img_ptr = mlx_xpm_file_to_image(game->var.mlx, filename, &game->intro.img[i].w, &game->intro.img[i].h);
		game->intro.img[i].addr = mlx_get_data_addr(game->intro.img[i].img_ptr, &game->intro.img[i].bpp, &game->intro.img[i].size_line, &game->intro.img[i].endian);
		if (!game->intro.img[i].img_ptr)
			return ;
		i++;
	}
}
