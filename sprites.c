/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:30:31 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/05 13:35:30 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

void	sort_sprites_by_distance(t_sprit *sp)
{
	int		i;
	int		j;
	t_sprit	temp;

	j = 0;
	while (j < g_global()->sp_count - 1)
	{
		i = 0;
		while (i < g_global()->sp_count - j - 1)
		{
			if (sp[i].dist < sp[i + 1].dist)
			{
				temp = sp[i];
				sp[i] = sp[i + 1];
				sp[i + 1] = temp;
			}
			i++;
		}
		j++;
	}
}

int	count_sprite(char **map)
{
	int i;
	int	j;
	int	count;

	j = 0;
	count = 0;
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == sprite)
				count++;
			i++;
		}
		j++;
	}
	return (count);
}
