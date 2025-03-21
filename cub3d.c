/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/21 15:26:55 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"

void	put_pixel(t_image *img, int x, int y, int color)
{
	int	offset;

	// char	*offset;
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	offset = (y * img->size_line) + (x * img->bpp / 8);
	img->addr[offset] = color & 0xFF;
	img->addr[offset + 1] = (color >> 8) & 0xFF;
	img->addr[offset + 2] = (color >> 16) & 0xFF;
	// offset = img->img_ptr + (y * img->size_line + x * (img->bpp / 8));
	// *(int *)offset = color;
}

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
	exit(0);
}

void	dda_algorithm(t_game *game, double x0, double y0, double x1, double y1)
{
	double	dx;
	double	dy;
	double	steps;
	double	xinc;
	double	yinc;
	int		i;

	dx = x1 - x0;
	dy = y1 - y0;
	if (dx > dy)
		steps = dx;
	else
		steps = dy;
	xinc = dx / (double)steps;
	yinc = dy / (double)steps;
	i = 0;
	while (i <= steps)
	{
		put_pixel(&game->img, x0 * 32, y0 * 32, 0XFF0000);
		x0 += xinc;
		y0 += yinc;
		i++;
	}
}

int	game_loop(t_game *game)
{
	int			x;
	t_vector	cam;
	t_vector	ray_dir;
	t_vect		map;
	t_vector	side;
	t_vector	delta;
	t_vect		step;
	bool		hit;
	int			s;
	double		wall_perp;
	int			line_h;
	int			draw_s;
	int			draw_e;
	int			color = 0x0000FF;

	// dda_algorithm(game, game->player.pos.x, game->player.pos.y, 0, 15);
	// mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr,
	x = 0;
	while (x < WIDTH)
	{
		cam.x = ((2 * x) / (double)WIDTH) - 1;
		ray_dir.x = game->player.dir.x + game->player.plan.x * cam.x;
		ray_dir.y = game->player.dir.y + game->player.plan.y * cam.x;
		map.x = (int)game->player.pos.x;
		map.y = (int)game->player.pos.y;
		// printf("map.x [%d] et map.y [%d]\n", map.x, map.y);
		hit = false;
		if (ray_dir.x == 0)
			delta.x = 1e30;
		else
			delta.x = fabs(1 / ray_dir.x);
		if (ray_dir.y == 0)
			delta.y = 1e30;
		else
			delta.y = fabs(1 / ray_dir.y);
		if (ray_dir.x < 0)
		{
			step.x = -1;
			side.x = (game->player.pos.x - map.x) * delta.x;
		}
		else
		{
			step.x = 1;
			side.x = (map.x + 1.0 - game->player.pos.x) * delta.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			side.y = (game->player.pos.y - map.y) * delta.y;
		}
		else
		{
			step.y = 1;
			side.y = (map.y + 1.0 - game->player.pos.y) * delta.y;
		}
		while (hit == false)
		{
			if (side.x < side.y)
			{
				side.x += delta.x;
				map.x += step.x;
				s = 0;
			}
			else
			{
				side.y += delta.y;
				map.y += step.y;
				s = 1;
			}
			// put_pixel(&game->img, map.x * 30, map.y * 30, 0xFF0000);
			if (game->map[map.y][map.x] == wall)
				hit = true;
		}
		// 	wall_perp = (side.x - delta.x);
		// else
		// 	wall_perp = (side.y - delta.y);
		if (s == 0)
			wall_perp = (map.x - game->player.pos.x + (1 - step.x) / 2)
					/ ray_dir.x;
		else
			wall_perp = (map.y - game->player.pos.y + (1 - step.y) / 2)
					/ ray_dir.y;
		line_h = (int)(HEIGHT / wall_perp);
		draw_s = -line_h / 2 + HEIGHT / 2;
		// printf("line h is %d\n", line_h);
		if (draw_s < 0)
			draw_s = 0;
		draw_e = line_h / 2 + HEIGHT / 2;
		if (draw_e >= HEIGHT)
			draw_e = HEIGHT - 1;
		// if (s == 1)
		// 	color = color / 2;
		// printf("draw s %d et draw e %d\n", draw_s, draw_e);
		while (draw_s <= draw_e)
		{
			// mlx_pixel_put(game->var.mlx, game->var.win, draw_s, x, 0x0000FF);
			put_pixel(&game->img, x, draw_s, color);
			draw_s++;
		}
		x++;
	}
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	return (0);
}

int	init_image(t_game *game, t_image *image)
{
	image->img_ptr = mlx_new_image(game->var.mlx, WIDTH, HEIGHT);
	if (!image->img_ptr)
		return (1);
	image->addr = mlx_get_data_addr(image->img_ptr, &image->bpp,
			&image->size_line, &image->endian);
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
	mlx_hook(game.var.win, DestroyNotify, 0, leave_windows, &game);
	// mlx_string_put(game.var.mlx, game.var.win, WIDTH / 2, HEIGHT - 120,
	// 0xFF0000, "PRESS THE START TO BEGIN");
	mlx_loop_hook(game.var.mlx, game_loop, &game);
	// game_loop(&game);
	mlx_loop(game.var.mlx);
	return (0);
}