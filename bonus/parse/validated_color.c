/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validated_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:43:49 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/17 13:22:43 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse_bonus.h"

int	check_id_rgb(const char *str, char id_rgb)
{
	while (white_space(*str))
		str++;
	if (*str != id_rgb || !white_space(*(str + 1)))
		return (0);
	return (1);
}

char	*get_rgb(const char *str, char id_rgb)
{
	while (white_space(*str))
		str++;
	if (*str != id_rgb)
		return (NULL);
	str++;
	if (!white_space(*str))
		return (NULL);
	while (white_space(*str))
		str++;
	return (ft_strtrim(str, " \t\n"));
}

int	is_valid_rgb_format(const char *str)
{
	int	digit_count;
	int	comma_count;

	digit_count = 0;
	comma_count = 0;
	while (*str)
	{
		if (ft_isdigit(*str))
		{
			digit_count++;
			if (digit_count > 3)
				return (0);
		}
		else if (*str == ',')
		{
			if (digit_count == 0)
				return (0);
			comma_count++;
			digit_count = 0;
		}
		else
			return (0);
		str++;
	}
	return (comma_count == 2 && digit_count > 0 && digit_count <= 3);
}

int	check_color_valid(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	if (g < 0 || g > 255)
		return (0);
	if (b < 0 || b > 255)
		return (0);
	return (1);
}

int	get_rgb_value(const char *str, int *r, int *g, int *b)
{
	char	**rgb_values;
	int		i;

	rgb_values = ft_split(str, ',');
	if (!rgb_values)
		return (0);
	i = 0;
	while (rgb_values[i])
		i++;
	if (i != 3)
		return (ft_free_arr((void **)rgb_values), 0);
	*r = ft_atoi(rgb_values[0]);
	*g = ft_atoi(rgb_values[1]);
	*b = ft_atoi(rgb_values[2]);
	ft_free_arr((void **)rgb_values);
	if (!check_color_valid(*r, *g, *b))
		return (0);
	return (1);
}
