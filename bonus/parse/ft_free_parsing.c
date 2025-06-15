/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:24:16 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 20:06:25 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse_bonus.h"

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
}

void	ft_free_parse(char **input, t_text *txt)
{
	ft_free_arr((void **)input);
	ft_free_txt(txt);
}

void	init_text(t_text *txt)
{
	txt->no_path = NULL;
	txt->so_path = NULL;
	txt->ea_path = NULL;
	txt->we_path = NULL;
	txt->ceiling = NULL;
	txt->floor = NULL;
	txt->f.r = 0;
	txt->f.g = 0;
	txt->f.b = 0;
	txt->c.r = 0;
	txt->c.g = 0;
	txt->c.b = 0;
}
