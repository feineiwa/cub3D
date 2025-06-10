/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   video.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:55:57 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/10 10:44:40 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	set_filename(char *filename, int j)
{
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
}

void	load_video(t_game *game)
{
	char	filename[21];
	int		i;
	int		j;

	ft_memcpy((char *)filename, (char *)"./video/intro000.xpm", 21);
	i = 0;
	game->intro.frm = 0;
	game->intro.last_time = get_current_time();
	while (i < 384)
	{
		j = i + 1;
		set_filename(filename, j);
		load_image(game, &game->intro.img[i], filename);
		i++;
	}
}
