/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/04/01 00:57:58 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"

bool	g_updated = false;

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

void	draw_line(t_image *img, t_vector a, t_vector b, int color)
{
	double	dx;
	double	dy;
	double	steps;
	double	xinc;
	double	yinc;
	int		i;

	dx = (b.x - a.x);
	dy = (b.y - a.y);
	steps = fmax(fabs(dx), fabs(dy));
	xinc = dx / steps;
	yinc = dy / steps;
	i = 0;
	while (i <= steps)
	{
		a.x += xinc;
		a.y += yinc;
		put_pixel(img, a.x, a.y, color);
		i++;
	}
}

double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * PI);
	if (angle < 0)
		angle += 2 * PI;
	return (angle);
}

void	move_player(t_game *game)
{
	if (game->player.key.rot_l == true)
		game->player.angle += game->player.turn_dir * ROT_SPEED;
	if (game->player.key.rot_r == true)
		game->player.angle += game->player.turn_dir * ROT_SPEED;
	// game->player.angle = normalize_angle(game->player.angle);
	if (game->player.key.key_up == true)
	{
	// // 	if (game->map[(int)game->player.pos.y][(int)(game->player.pos.x
	// // 			+ game->player.dir.x * MOVE_SPEED)] == '0')
	// // 		game->player.pos.x += game->player.dir.x * MOVE_SPEED;
	// // 	if (game->map[(int)(game->player.pos.y + game->player.dir.y
	// // 			* MOVE_SPEED)][(int)game->player.pos.x] == '0')
	// // 		game->player.pos.y += game->player.dir.y * MOVE_SPEED;
		game->player.pos.x += cos(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		// printf("cos is %f\n", cos(game->player.angle));
		// printf("sin is %f\n", sin(game->player.angle));
		game->player.pos.y += sin(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		// printf("player angle is '%f'\n", (game->player.angle * 180) / PI);
	}
	if (game->player.key.key_down == true)
	{
	// // 	if (game->map[(int)game->player.pos.y][(int)(game->player.pos.x
	// // 			- game->player.dir.x * MOVE_SPEED)] == '0')
	// // 		game->player.pos.x += game->player.dir.x * MOVE_SPEED;
	// // 	if (game->map[(int)(game->player.pos.y - game->player.dir.y
	// // 			* MOVE_SPEED)][(int)game->player.pos.x] == '0')
	// // 		game->player.pos.y += game->player.dir.y * MOVE_SPEED;
		game->player.pos.x += cos(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		game->player.pos.y += sin(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
	}
	if (game->player.key.key_left == true)
	{
	// // 	if (game->map[(int)game->player.pos.y][(int)(game->player.pos.x
	// // 			- game->player.dir.x * MOVE_SPEED)] == '0')
	// // 		game->player.pos.x += game->player.dir.x * MOVE_SPEED;
	// // 	if (game->map[(int)(game->player.pos.y - game->player.dir.y
	// // 			* MOVE_SPEED)][(int)game->player.pos.x] == '0')
	// // 		game->player.pos.y += game->player.dir.y * MOVE_SPEED;
		game->player.angle += PI / 2;
		game->player.pos.x += cos(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		game->player.pos.y += sin(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		game->player.angle -= PI / 2;
	}
	if (game->player.key.key_right == true)
	{
	// // 	if (game->map[(int)game->player.pos.y][(int)(game->player.pos.x
	// // 			- game->player.dir.x * MOVE_SPEED)] == '0')
	// // 		game->player.pos.x += game->player.dir.x * MOVE_SPEED;
	// // 	if (game->map[(int)(game->player.pos.y - game->player.dir.y
	// // 			* MOVE_SPEED)][(int)game->player.pos.x] == '0')
	// // 		game->player.pos.y += game->player.dir.y * MOVE_SPEED;
		game->player.angle += PI / 2;
		game->player.pos.x += cos(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		game->player.pos.y += sin(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
		game->player.angle -= PI / 2;
	}
	// game->player.pos.x += cos(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
	// game->player.pos.y += sin(game->player.angle) * game->player.walk_dir * MOVE_SPEED;
}

void	clear_image(t_image *img, int width, int height)
{
	int	x;
	int	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			put_pixel(img, x, y, 0x333333);
			x++;
		}
		y++;
	}
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

int	ft_key_press(int keysym, t_game *game)
{
	if (g_updated == true)
		return (0);
	if (keysym == XK_w)
	{
		game->player.key.key_up = true;
		game->player.walk_dir = 1;
		game->player.dir.y = -1;
		game->player.dir.x = 0;
	}
	if (keysym == XK_s)
	{
		game->player.key.key_down = true;
		game->player.walk_dir = -1;
		game->player.dir.y = 1;
		game->player.dir.x = 0;
	}
	if (keysym == XK_d)
	{
		game->player.key.key_right = true;
		game->player.dir.y = 0;
		game->player.dir.x = 1;
		game->player.walk_dir = 1;
	}
	if (keysym == XK_a)
	{
		game->player.key.key_left = true;
		game->player.dir.y = 0;
		game->player.dir.x = -1;
		game->player.walk_dir = -1;
	}
	if (keysym == XK_Left)
	{
		game->player.key.rot_l = true;
		game->player.turn_dir = -1;
	}
	if (keysym == XK_Right)
	{
		game->player.key.rot_r = true;
		game->player.turn_dir = 1;
	}
	return (0);
}

int	ft_key_release(int keysym, t_game *game)
{
	if (g_updated == true)
		return (1);
	if (keysym == XK_w)
		game->player.key.key_up = false;
	if (keysym == XK_s)
		game->player.key.key_down = false;
	if (keysym == XK_d)
		game->player.key.key_right = false;
	if (keysym == XK_a)
		game->player.key.key_left = false;
	if (keysym == XK_Left)
	{
		game->player.key.rot_l = false;
		game->player.turn_dir = 0;
	}
	if (keysym == XK_Right)
	{
		game->player.key.rot_r = false;
		game->player.turn_dir = 0;
	}
	game->player.walk_dir = 0;
	game->player.dir.x = 0;
	game->player.dir.y = 0;
	return (0);
}

void	draw_square(t_image *img, int x, int y, int size, int color)
{
	int	j;
	int	i;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size)
		{
			put_pixel(img, i + x, j + y, color);
			i++;
		}
		j++;
	}
}

bool	is_wall(int x, int y, char **map)
{
	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return (false);
	if (map[(int)(y / TILE_SIZE)][(int)(x / TILE_SIZE)] == wall)
		return (true);
	return (false);
}

void	draw_circle(t_image *img, t_vector p, int radius, int color)
{
	int	i;
	int	j;
	int	dist_squared;

	j = -radius;
	put_pixel(img, p.x, p.y, color);
	while (j <= radius)
	{
		i = -radius;
		while (i <= radius)
		{
			dist_squared = (i * i) + (j * j);
			if ((dist_squared >= ((radius * radius) - radius)) && (dist_squared <= ((radius * radius) + radius)))
				put_pixel(img, p.x + i, p.y + j, color);
			i++;
		}
		j++;
	}
	// double	angle;

	// angle = 0;
	// put_pixel(img, p.x, p.y, 0xFF0000);
	// printf("px {%f} and py {%f}\n", p.x, p.y);
	// while (angle <= 360)
	// {
	// 	p.x += cos(angle) * radius;
	// 	p.y += sin(angle) * radius;
	// 	printf("px [%f] and py [%f]\n", p.x, p.y);
	// 	put_pixel(img, p.x, p.y, color);
	// 	angle += 1;
	// }
}

void	draw_map(t_image *img, char **map)
{
	int	x;
	int	y;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == wall)
				draw_square(img, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE - 1,
					0x000000);
			if (map[y][x] != wall)
				draw_square(img, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
					0xFFFFFF);
			x++;
		}
		y++;
	}
}

bool	distance_between_points(t_vector a, t_vector b)
{
	return (sqrt(((b.x - a.x) * (b.x - a.x)) + (((b.y - a.y) * ((b.y - a.y))))));
}

int	game_loop(t_game *game)
{
	t_vector	a;
	t_vector	b;

	g_updated = true;
	move_player(game);
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT);
	int	ray = 0;
	double	ray_angle;
	draw_map(&game->img, game->map);
	a.x = game->player.pos.x * TILE_SIZE;
	a.y = game->player.pos.y * TILE_SIZE;
	ray_angle = normalize_angle(game->player.angle - (FOV / 2));
	int	is_facing_down = 0;
	int	is_facing_left = 0;
	int	is_facing_right = 0;
	int	is_facing_up = 0;
	// printf("%f\n", ray_angle * 180 / PI);
	// printf("ay is %f\n", floor(game->player.pos.y) - 1);
	// printf("ay is %f\n", ceil(game->player.pos.y)  + 1);
	// printf("ay is %f\n", (floor(a.y / TILE_SIZE) * (TILE_SIZE) + TILE_SIZE) / TILE_SIZE);
	// Ax = (Ay - game->player.pos.y) / tan(alpha) + game->player.pos.x;
	while (ray <= (int)R_NB)
	{
		is_facing_down = 0;
		is_facing_left = 0;
		is_facing_right = 0;
		is_facing_up = 0;
		ray_angle = normalize_angle(ray_angle);
		if (ray_angle > 0 && ray_angle < PI)
			is_facing_down = 1;
		else
			is_facing_up = 1;
		if (ray_angle > PI / 2  && ray_angle < 3 * PI / 2)
			is_facing_left = 1;
		else
			is_facing_right = 1;
		t_vector horiz_p = {0, 0};
		t_vector first_int = {0, 0};
		if (is_facing_up == 1)
			first_int.y = floor(a.y / TILE_SIZE) * TILE_SIZE - 1;
		else if (is_facing_down == 1)
			first_int.y = floor(a.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		first_int.x = a.x + (first_int.y - a.y) / tan(ray_angle);
		
		t_vector next_horiz = {first_int.x, first_int.y};
		t_vector s = {0, 0};
		if (is_facing_up == 1)
			s.y = - TILE_SIZE;
		if (is_facing_down == 1)
			s.y = TILE_SIZE;
		s.x = s.y / tan(ray_angle);
		bool	found_horizontal = false;
		while (next_horiz.x <= WIDTH && next_horiz.x >= 0 && next_horiz.y <= HEIGHT && next_horiz.y >= 0)
		{
			if (is_wall(next_horiz.x, next_horiz.y, game->map))
			{
				found_horizontal = true;
				horiz_p.x = next_horiz.x;
				horiz_p.y = next_horiz.y;
				break ;
			}
			else
			{
				next_horiz.x += s.x;
				next_horiz.y += s.y;
			}
		}
		t_vector vert_p = {0, 0};
		if (is_facing_right == 1)
			first_int.x = floor(a.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		else if (is_facing_left == 1)
			first_int.x = floor(a.x / TILE_SIZE) * TILE_SIZE - 1;
		first_int.y = a.y + (first_int.x - a.x) * tan(ray_angle);
		
		t_vector next_vert = {first_int.x, first_int.y};
		s.x = 0;
		s.y = 0;
		if (is_facing_right == 1)
			s.x = TILE_SIZE;
		else if (is_facing_left == 1)
			s.x = -TILE_SIZE;
		s.y = s.x * tan(ray_angle);
		bool	found_vertical = false;
		while (next_vert.x <= WIDTH && next_vert.x >= 0 && next_vert.y <= HEIGHT && next_vert.y >= 0)
		{
			if (is_wall(next_vert.x, next_vert.y, game->map))
			{
				found_vertical = true;
				vert_p.x = next_vert.x;
				vert_p.y = next_vert.y;
				break ;
			}
			else
			{
				next_vert.x += s.x;
				next_vert.y += s.y;
			}
		}
		//distance calculation

		double horizontal_dist = 0;
		double vertical_dist = 0;
		if (found_horizontal == true)
			horizontal_dist = distance_between_points(a, horiz_p);
		else
			horizontal_dist = 99999.0;
		if (found_vertical == true)
			vertical_dist = distance_between_points(a, vert_p);
		else
			vertical_dist = 99999.0;
		t_vector wall_hit = {0, 0};
		if (horizontal_dist < vertical_dist)
		{
			wall_hit.x = horiz_p.x;
			wall_hit.y = horiz_p.y;
		}
		else
		{
			wall_hit.x = vert_p.x;
			wall_hit.y = vert_p.y;
			
		}
		draw_line(&game->img, a, wall_hit, 0xFF00FF);
		ray_angle += FOV / R_NB;
		ray++;
	}
	// printf("player x %f and player y %f\n", game->player.pos.x, game->player.pos.y);
	draw_circle(&game->img, a, 10, 0x00FF00);
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	g_updated = false;
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
	mlx_put_image_to_window(game.var.mlx, game.var.win, game.img.img_ptr, 0,
		0);
	// mlx_string_put(game.var.mlx, game.var.win, WIDTH / 2, HEIGHT - TILE_SIZE,
	// 0xFF0000, "PRESS THE START TO BEGIN");
	g_updated = false;
	mlx_hook(game.var.win, DestroyNotify, 0, leave_windows, &game);
	mlx_hook(game.var.win, KeyPress, KeyPressMask, ft_key_press, &game);
	mlx_hook(game.var.win, KeyRelease, KeyReleaseMask, ft_key_release, &game);
	mlx_loop_hook(game.var.mlx, game_loop, &game);
	// game_loop(&game);
	mlx_loop(game.var.mlx);
	return (0);
}