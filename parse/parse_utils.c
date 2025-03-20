/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:25:21 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/20 14:39:20 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	ft_ispace(char c)
{
	if (c == 32 || (c >= 7 && c <= 13))
		return (1);
	return (0);
}

int	ft_skip_space(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (ft_ispace(s[i]))
		i++;
	return (i);
}

int	ft_array_lenght(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

