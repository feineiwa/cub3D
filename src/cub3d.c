/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:04:31 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/14 09:25:24 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

t_g	*g_global(void)
{
	static t_g	value = {-1, false, 0, 0.f, 1.0f, 875, false, 0, 0, false,
		false, 0, 0.f, 100, true, 0, -1};

	return (&value);
}

void	event_listener(t_game *game)
{
	mlx_hook(game->var.win, DestroyNotify, 0, leave_windows, game);
	mlx_hook(game->var.win, KeyPress, KeyPressMask, ft_key_press, game);
	mlx_hook(game->var.win, KeyRelease, KeyReleaseMask, ft_key_release, game);
	mlx_loop_hook(game->var.mlx, game_loop, game);
	mlx_loop(game->var.mlx);
}

int	parse_main(char *av, char ***map, t_text *txt)
{
	char **input;

	input = map_to_char(av);
	if (!parsing(input, txt, map))
	{
		ft_putendl_fd("Error\nPlease check the map", 2);
		ft_free_arr((void **)input);
		ft_free_txt(txt);
		return (1);
	}
	ft_free_arr((void **)input);
	if (!(is_valid_rgb_format(txt->ceiling)) || !get_rgb_value(txt->ceiling,
			&txt->c.r, &txt->c.g, &txt->c.b))
	{
		ft_putendl_fd("Error\ncheck color", 2);
		ft_free_arr((void **)map);
		ft_free_txt(txt);
		return (1);
	}
	if (!(is_valid_rgb_format(txt->floor)) || !get_rgb_value(txt->floor, &txt->f.r,
			&txt->f.g, &txt->f.b))
	{
		ft_putendl_fd("Error\ncheck color", 2);
		ft_free_arr((void **)*map);
		ft_free_txt(txt);
		return (1);
	}
	if (check_blank_line_map(*map))
	{
		ft_putendl_fd("Error\ncheck your map", 2);
		ft_free_arr((void **)*map);
		ft_free_txt(txt);
		return (1);
	}
	padding_space(*map);
	if (!validated_map(*map))
	{
		ft_putendl_fd("Error\ncheck your map", 2);
		ft_free_arr((void **)*map);
		ft_free_txt(txt);
		return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (!check_arguments(ac, av))
		return (1);
	game.bonus = false;
	init_game(&game);
	init_text(&game.txt);
	if (parse_main(av[1], &game.map, &game.txt))
		return (1);
	if (!init_window(&game))
	{
		ft_free_arr((void **)game.map);
		return (ft_free_txt(&game.txt), 1);
	}
	init_player(&game);
	if (!init_image_win(&game, &game.img, WIDTH, HEIGHT))
		return (free_game_var(&game), 1);
	init_all_images(&game);
	load_all_images(&game);
	event_listener(&game);
	return (0);
}
