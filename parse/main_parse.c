/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:35:25 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/14 09:26:40 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include <stdio.h>

int	ft_array_lenght(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

// void	print_tab(char **tab)
// {
// 	int	i;

// 	i = 0;
// 	if (!tab)
// 		return ;
// 	while (tab[i])
// 	{
// 		write(1, tab[i], ft_strlen(tab[i]));
// 		write(1, "\n", 1);
// 		i++;
// 	}
// }

int	check_blank_line_map(char **map)
{
	int	j;

	j = 0;
	while (map[j])
	{
		if (white_line(map[j]))
			return (1);
		j++;
	}
	return (0);
}

void	replace_new_line_space(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			s[i] = ' ';
		i++;
	}
}

char	*ft_pad_space(char *map, int w)
{
	char	*s;
	int		len;

	len = (int)ft_strlen(map);
	s = (char *)ft_calloc(sizeof(char), w + 1);
	if (!s)
		return (map);
	ft_strlcpy(s, map, len + 1);
	if (len < w)
	{
		while (len < w)
			s[len++] = ' ';
		s[len] = 0;
	}
	replace_new_line_space(s);
	free(map);
	return (s);
}

void	padding_space(char **map)
{
	int	len;
	int	w;
	int	i;

	w = 0;
	i = 0;
	while (map[i])
	{
		len = (int)ft_strlen(map[i]);
		if (w < len)
			w = len;
		i++;
	}
	i = 0;
	while (map[i])
	{
		map[i] = ft_pad_space(map[i], w);
		i++;
	}
}

// int	main(int argc, char **argv)
// {
// 	char	**input;
// 	char	**map;
// 	t_text	txt;

// 	map = NULL;
// 	if (!check_arguments(argc, argv))
// 		return (1);
// 	init_text(&txt);
// 	input = map_to_char(argv[1]);
// 	if (!input)
// 		return (ft_putendl_fd("Error\nEmpty file", 2), 1);
// 	if (!parsing(input, &txt, &map))
// 	{
// 		ft_putendl_fd("Error\nPlease check the map", 2);
// 		ft_free_arr(input);
// 		ft_free_txt(&txt);
// 		return (1);
// 	}
// 	ft_free_arr(input);
// 	if (!(is_valid_rgb_format(txt.ceiling)) || !get_rgb_value(txt.ceiling,
// 			&txt.c.r, &txt.c.g, &txt.c.b))
// 	{
// 		ft_putendl_fd("Error\ncheck color", 2);
// 		ft_free_arr(map);
// 		ft_free_txt(&txt);
// 		return (1);
// 	}
// 	if (!(is_valid_rgb_format(txt.floor)) || !get_rgb_value(txt.floor, &txt.f.r,
// 			&txt.f.g, &txt.f.b))
// 	{
// 		ft_putendl_fd("Error\ncheck color", 2);
// 		ft_free_arr(map);
// 		ft_free_txt(&txt);
// 		return (1);
// 	}
// 	if (check_blank_line_map(map))
// 	{
// 		ft_putendl_fd("Error\ncheck your map", 2);
// 		ft_free_arr(map);
// 		ft_free_txt(&txt);
// 		return (1);
// 	}
// 	padding_space(map);
// 	if (!validated_map(map))
// 	{
// 		ft_putendl_fd("Error\ncheck your map", 2);
// 		ft_free_arr(map);
// 		ft_free_txt(&txt);
// 		return (1);
// 	}
// 	ft_free_arr(map);
// 	ft_free_txt(&txt);
// 	return (0);
// }
