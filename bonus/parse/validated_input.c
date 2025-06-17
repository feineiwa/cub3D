/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validated_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:19:24 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/17 13:22:55 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse_bonus.h"

int	check_id_path(const char *str, char *id_path)
{
	while (white_space(*str))
		str++;
	if (*str != *id_path || *(str + 1) != *(id_path + 1) || !white_space(*(str
				+ 2)))
		return (0);
	return (1);
}

int	check_arguments(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3d <map.cub>\n", 2);
		return (0);
	}
	if (ft_strlen(argv[1]) <= 4 || ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4,
			".cub", 4) != 0)
	{
		ft_putstr_fd("Error\nInvalid file extension. Expected .cub\n", 2);
		return (0);
	}
	if (ft_strncmp(argv[1] + ft_strlen(argv[1]) - 5, "/.cub", 5) == 0)
	{
		ft_putstr_fd("Error\nInvalid file name.\n", 2);
		return (0);
	}
	return (1);
}
