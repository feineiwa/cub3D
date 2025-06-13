/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:04:31 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:16:28 by frahenin         ###   ########.fr       */
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

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (ft_putendl_fd("Error\nArguments should be just 2", 2), 1);
	game.bonus = false;
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
	if (!init_image_win(&game, &game.img, WIDTH, HEIGHT))
		return (free_game_var(&game), 1);
	init_all_images(&game);
	load_all_images(&game);
	event_listener(&game);
	return (0);
}
