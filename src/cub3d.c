/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/09 17:43:48 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	event_listener(t_game *game)
{
	launch_music("./sounds/disaster_war.mp3", "0.5");
	mlx_hook(game->var.win, DestroyNotify, 0, leave_windows, game);
	mlx_hook(game->var.win, KeyPress, KeyPressMask, ft_key_press, game);
	mlx_hook(game->var.win, KeyRelease, KeyReleaseMask, ft_key_release, game);
	mlx_hook(game->var.win, ButtonPress, ButtonPressMask, ft_mouse_pressed,
		game);
	mlx_hook(game->var.win, ButtonRelease, ButtonReleaseMask, ft_mouse_release,
		game);
	mlx_hook(game->var.win, MotionNotify, PointerMotionMask, ft_mouse_move,
		game);
	mlx_loop_hook(game->var.mlx, game_loop, game);
	mlx_loop(game->var.mlx);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (ft_putendl_fd("Error\nArguments should be just 2", 2), 1);
	init_game(&game);
	game.txt = ft_parsing(av[1], &game.map);
	if (game.map == NULL)
		return (0);
	if (!init_window(&game))
	{
		ft_free_arr((void **)game.map);
		return (ft_free_txt(&game.txt), 1);
	}
	init_player(&game);
	game.doors = handle_door(game.map);
	if (!init_image_win(&game, &game.img, WIDTH, HEIGHT))
		return (free_game_var(&game), 1);
	init_all_images(&game);
	load_all_images(&game);
	g_global()->sp_count = count_sprite(game.map);
	game.sp = init_sprite(game.map);
	event_listener(&game);
	return (0);
}
