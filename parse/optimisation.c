/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:42 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/16 14:55:06 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"

static int	get_right_zero(char **map)
{
	int	i;
	int	j;
	int	right_zero;

	right_zero = 0;
	i = 0;
	while (map[i])
	{
		j = ft_strlen(map[i]) - 1;
		while (j >= 0)
		{
			if ((map[i][j] == '0' || count_player(map[i][j])) && (j
					+ 1) > right_zero)
				right_zero = j + 1;
			j--;
		}
		i++;
	}
	return (right_zero);
}

static int	get_top_zero(char **map)
{
	int	i;
	int	j;
	int	top_zero;

	top_zero = 2147483647;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if ((map[i][j] == '0' || count_player(map[i][j])) && i < top_zero)
				top_zero = i;
			j++;
		}
		i++;
	}
	return (top_zero);
}

static int	get_left_zero(char **map)
{
	int	i;
	int	j;
	int	left_zero;

	left_zero = 2147483647;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if ((map[i][j] == '0' || count_player(map[i][j])) && j < left_zero)
				left_zero = j;
			j++;
		}
		i++;
	}
	return (left_zero);
}

static int	get_bottom_zero(char **map)
{
	int	i;
	int	j;
	int	bottom_zero;

	bottom_zero = 0;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if ((map[i][j] == '0' || count_player(map[i][j])) && (i
					+ 1) > bottom_zero)
				bottom_zero = i + 1;
			j++;
		}
		i++;
	}
	return (bottom_zero);
}

char	**optimisation(char **map)
{
	t_mapping	mapping;
	int			i;
	char		**new_map;

	mapping.top = get_top_zero(map) - 1;
	mapping.bottom = get_bottom_zero(map) + 1;
	mapping.left = get_left_zero(map) - 1;
	mapping.right = get_right_zero(map) + 1;
	new_map = malloc(sizeof(char *) * (mapping.bottom - mapping.top + 1));
	if (!new_map)
		return (NULL);
	i = 0;
	while (mapping.top + i < mapping.bottom)
	{
		new_map[i] = ft_substr(map[mapping.top + i], mapping.left, mapping.right
				- mapping.left);
		if (!new_map[i])
		{
			ft_free_arr((void **)new_map);
			return (NULL);
		}
		i++;
	}
	new_map[i] = NULL;
	return (ft_free_arr((void **)map), new_map);
}
