/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/20 17:56:32 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"

void	ft_putarr_endl(char **arr, int fd)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		ft_putendl_fd(arr[i], fd);
		i++;
	}
}

int	init_window(t_game *game)
{
	game->var.mlx = mlx_init();
	if (!game->var.mlx)
		return (1);
	game->var.win = mlx_new_window(game->var.mlx, WIDTH, HEIGHT, "Cub3d");
	if (!game->var.win)
	{
		mlx_destroy_display(game->var.mlx);
		ft_free(game->var.mlx);
		return (1);
	}
	return (0);
}

int	leave_windows(t_game *game)
{
	ft_free_txt(&game->txt);
	mlx_clear_window(game->var.mlx, game->var.win);
	mlx_destroy_image(game->var.mlx, game->img.img_ptr);
	mlx_destroy_window(game->var.mlx, game->var.win);
	game->var.win = NULL;
	mlx_destroy_display(game->var.mlx);
	game->var.mlx = ft_free(game->var.mlx);
	exit (0);
}

// int	game_loop(t_game *game)
// {
	
// 	return (0);
// }

int	init_image(t_game *game, t_image *image)
{
	image->img_ptr = mlx_new_image(game->var.mlx, WIDTH, HEIGHT);
	if (!image->img_ptr)
		return (1);
	image->addr = mlx_get_data_addr(image->img_ptr, &image->bpp, &image->size_line, &image->endian);
	return (0);
}

int	main(int ac, char **av)
{
	t_game game;

	if (ac != 2)
	{
		ft_putendl_fd("Error\nArguments should be just 2", 2);
		return (1);
	}
	game.txt = ft_parsing(av[1]);
	game.map = game.txt.map;
	if (game.map == NULL)
		return (0);
	if (init_window(&game))
		return (ft_free_txt(&game.txt), 1);
	init_player(&game);
	if (init_image(&game, &game.img))
	{
		ft_free_txt(&game.txt);
		leave_windows(&game);
	}
	mlx_put_image_to_window(game.var.mlx, game.var.win, game.img.img_ptr, 0, 0);
	// ft_putarr_endl(game.map, 1);
	mlx_hook(game.var.win, DestroyNotify, 0, leave_windows,
		&game);
	mlx_string_put(game.var.mlx, game.var.win, )
	mlx_loop_hook(game.var.mlx, game_loop, &game);
	mlx_loop(game.var.mlx);
	return (0);
}