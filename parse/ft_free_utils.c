/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:24:16 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/20 14:58:31 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	*ft_free(void *param)
{
	if (param)
		free(param);
	param = NULL;
	return (param);
}

void	ft_free_arr(void **arr)
{
	int	i;

	i = 0;
	if (*arr == NULL)
		return ;
	while (arr[i])
	{
		ft_free(arr[i]);
		i++;
	}
	ft_free(arr);
	arr = NULL;
}

void	ft_free_txt(t_text *txt)
{
	if (txt->ea_path)
		ft_free(txt->ea_path);
	if (txt->no_path)
		ft_free(txt->no_path);
	if (txt->we_path)
		ft_free(txt->we_path);
	if (txt->so_path)
		ft_free(txt->so_path);
	if (txt->ceiling)
		ft_free(txt->ceiling);
	if (txt->floor)
		ft_free(txt->floor);
	if (txt->map)
		ft_free_arr((void **)txt->map);
}
