/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/04/02 12:53:34 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"

bool	g_updated = false;
bool	g_first = false;

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

bool is_exceeded(int x, int y, t_player player)
{
	if (x < 0 || y < 0 || x >= player.w || y >= player.h)
		return (true);
	return(false);
}

void	move_player(t_game *game)
{
	t_vector move;
	double	move_speed;

	if (game->player.key.rot_l == true)
		game->player.angle += game->player.turn_dir * ROT_SPEED;
	if (game->player.key.rot_r == true)
		game->player.angle += game->player.turn_dir * ROT_SPEED;
	move_speed = MOVE_SPEED;
	if (game->player.key.speed == true)
		move_speed += MOVE_SPEED;
	if (game->player.key.key_up == true || game->player.key.key_down == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle) * game->player.walk_dir * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle) * game->player.walk_dir * move_speed;
		if (!is_exceeded((int)floor(move.x), (int)floor(move.y), game->player) && game->map[(int)floor(move.y)][(int)floor(move.x)] != wall)
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
	}
	if (game->player.key.key_left == true || game->player.key.key_right)
	{
		move.x = game->player.pos.x + cos(game->player.angle + 0.5 * PI) * game->player.walk_dir * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle + 0.5 * PI) * game->player.walk_dir * move_speed;
		if (!is_exceeded((int)floor(move.x), (int)floor(move.y), game->player) && game->map[(int)floor(move.y)][(int)floor(move.x)] != wall)
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
	}
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
	if (keysym == XK_w)
	{
		game->player.key.key_up = true;
		game->player.walk_dir = 1;
	}
	if (keysym == XK_s)
	{
		game->player.key.key_down = true;
		game->player.walk_dir = -1;
	}
	if (keysym == XK_d)
	{
		game->player.key.key_right = true;
		game->player.walk_dir = 1;
	}
	if (keysym == XK_a)
	{
		game->player.key.key_left = true;
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
	if (keysym == XK_Shift_L)
		game->player.key.speed = true;
	g_updated = true;
	return (0);
}

int	ft_key_release(int keysym, t_game *game)
{
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
	if (keysym == XK_Shift_L)
		game->player.key.speed = false;
	game->player.walk_dir = 0;
	game->player.dir.x = 0;
	game->player.dir.y = 0;
	g_updated = true;
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

bool	is_wall(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == wall)
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

double	distance_between_points(t_vector a, t_vector b)
{
	return (sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)));
}

void	draw_rectangle(t_image *image, int	x0, int x1, int y0, int y1, int color)
{
	int	i;

	while (y0 <= y1)
	{
		i = 0;
		while (i <= x1)
		{
			put_pixel(image, i + x0, y0, color);
			i++;
		}
		y0++;
	}
}

int	set_color_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int	game_loop(t_game *game)
{
	t_vector	a;
	t_vector	b;
	bool	is_facing_down = false;
	bool	is_facing_left = false;
	bool	is_facing_right = false;
	bool	is_facing_up = false;
	
	if (g_updated == false && g_first == false)
		return (1);
	move_player(game);
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT);
	double	ray_angle;
	// draw_map(&game->img, game->map);
	a.x = game->player.pos.x * TILE_SIZE;
	a.y = game->player.pos.y * TILE_SIZE;
	ray_angle = normalize_angle(game->player.angle - (FOV / 2));
	int	ray = 0;
	while (ray < R_NB)
	{
		ray_angle = normalize_angle(ray_angle);
		is_facing_down = false;
		is_facing_left = false;
		is_facing_right = false;
		is_facing_up = false;
		if (ray_angle > 0 && ray_angle < PI)
			is_facing_down = true;
		else
			is_facing_up = true;
		if (ray_angle < (0.5 * PI) || ray_angle > 1.5 * PI)
			is_facing_right = true;
		else
			is_facing_left = true;
		t_vector horiz_p = {0, 0};
		t_vector first_int = {0, 0};
		if (is_facing_up == 1)
			first_int.y = (floor(game->player.pos.y) - 1e-6) * TILE_SIZE;
		else if (is_facing_down == 1)
			first_int.y = floor(game->player.pos.y) * TILE_SIZE + TILE_SIZE;
		first_int.x = a.x + (first_int.y - a.y) / tan(ray_angle);
		
		t_vector next_horiz = {first_int.x, first_int.y};
		t_vector s = {0, 0};
		if (is_facing_up == 1)
			s.y = - TILE_SIZE;
		if (is_facing_down == 1)
			s.y = TILE_SIZE;
		s.x = s.y / tan(ray_angle);
		bool	found_horizontal = false;
		while (next_horiz.x < WIDTH && next_horiz.x > 0 && next_horiz.y < HEIGHT && next_horiz.y >= 0)
		{
			if (is_wall((int)floor(next_horiz.x), (int)floor(next_horiz.y), game->map, game->player))
			{
				found_horizontal = true;
				horiz_p = next_horiz;
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
			first_int.x = floor(game->player.pos.x) * TILE_SIZE + TILE_SIZE;
		else if (is_facing_left == 1)
			first_int.x = (floor(game->player.pos.x) - 1e-6) * TILE_SIZE;
		first_int.y = a.y + ((first_int.x - a.x) * tan(ray_angle));
		
		t_vector next_vert = {first_int.x, first_int.y};
		s.x = 0;
		s.y = 0;
		if (is_facing_right == 1)
			s.x = TILE_SIZE;
		else if (is_facing_left == 1)
			s.x = -TILE_SIZE;
		s.y = s.x * tan(ray_angle);
		
		bool	found_vertical = false;
		while (next_vert.x < WIDTH && next_vert.x >= 0 && next_vert.y < HEIGHT && next_vert.y >= 0)
		{
			if (is_wall((int)floor(next_vert.x), (int)floor(next_vert.y), game->map, game->player))
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
		// distance calculation

		double horizontal_dist = 0;
		double vertical_dist = 0;
		if (found_horizontal == true)
			horizontal_dist = distance_between_points(a, horiz_p);
		else
			horizontal_dist = __DBL_MAX__;
		if (found_vertical == true)
			vertical_dist = distance_between_points(a, vert_p);
		else
			vertical_dist = __DBL_MAX__;
		t_vector wall_hit = {0, 0};
		double		dist_wall = 0;
		int	color = 0;
		int	light_level = 60;
		color = 255;
		if (horizontal_dist < vertical_dist)
		{
			wall_hit = horiz_p;
			dist_wall = fabs(horizontal_dist);
			
		}
		else
		{
			wall_hit = vert_p;
			dist_wall = fabs(vertical_dist);
		}
		dist_wall *= cos(game->player.angle - ray_angle);
		if (dist_wall < 0.001)
			dist_wall = 0.001;
		color *= light_level / dist_wall;
		if (color >= 255)
			color = 255;
		else if (color <= 0)
			color = 0;
		if (is_facing_up && horizontal_dist < vertical_dist)
			color = set_color_rgb(0, color, 0);
		else if (is_facing_down && horizontal_dist < vertical_dist)
			color = set_color_rgb(color, 0, 0);
		else if (is_facing_right && horizontal_dist > vertical_dist)
			color = set_color_rgb(0, 0, color);
		else if (is_facing_left && horizontal_dist > vertical_dist)
			color = set_color_rgb(0, color, color);
		double waves = 4 + (cos (MOVE_SPEED + 8 * (game->player.pos.x + game->player.pos.y)));
		// printf("dist wall is %f\n", dist_wall);
		float	line_height = (TILE_SIZE / dist_wall) * 250;
		// printf("line height is %f\n", line_height);
		float	draw_begin = 0.5 * (HEIGHT - line_height) + waves;
		if (draw_begin < 0)
			draw_begin = 0;
		float	draw_end = draw_begin + line_height + waves;
		if (draw_end >= HEIGHT)
			draw_end = HEIGHT - 1;
		// printf("draw_begin is %f and the end is %f\n", draw_begin, draw_end);
		// printf("facing up %d\n", is_facing_up);
		// printf("facing down %d\n", is_facing_down);
		// printf("facing left %d\n", is_facing_left);
		// printf("facing right %d\n", is_facing_right);
		// printf("ray number %d is %f\n", ray, ray_angle);
		draw_rectangle(&game->img, ray * RES, RES, (int)(draw_begin), (int)(draw_end), color);
		// draw_line(&game->img, a, wall_hit, 0xFFEE8C);
		// int	j = 0;
		// while (j < WIDTH)
		// {
		// 	put_pixel(&game->img, ray * RES, j, 0xFF0000);
		// 	j++;
		// }
		ray_angle += FOV / R_NB;
		ray++;
	}
	t_vector center = {WIDTH / 2, (HEIGHT / 4) + HEIGHT /3};
	draw_circle(&game->img, center, 10, 0xFF0000);
	// draw_circle(&game->img, a, 5, 0x00FF00);
	t_vector compass = {WIDTH - 100, 100};
	t_vector needle = {compass.x + cos(game->player.angle) * 40, compass.y + sin(game->player.angle) * 40};
	draw_circle(&game->img, compass, 50, 0xFFFFFF);
	draw_line(&game->img, compass, needle, 0xFFFFFF);
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	mlx_string_put(game->var.mlx, game->var.win, compass.x - 10, compass.y - 60, 0xFFFFFF, "NORD");
	mlx_string_put(game->var.mlx, game->var.win, compass.x - 10, compass.y + 70, 0xFFFFFF, "SOUTH");
	mlx_string_put(game->var.mlx, game->var.win, compass.x + 60, compass.y + 5, 0xFFFFFF, "EAST");
	mlx_string_put(game->var.mlx, game->var.win, compass.x - 80, compass.y + 5, 0xFFFFFF, "WEST");
	g_updated = false;
	g_first = false;
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
	
	game.txt = ft_parsing(av[1], &game.map);
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
	g_first = true;
	mlx_hook(game.var.win, DestroyNotify, 0, leave_windows, &game);
	mlx_hook(game.var.win, KeyPress, KeyPressMask, ft_key_press, &game);
	mlx_hook(game.var.win, KeyRelease, KeyReleaseMask, ft_key_release, &game);
	mlx_loop_hook(game.var.mlx, game_loop, &game);
	// game_loop(&game);
	mlx_loop(game.var.mlx);
	return (0);
}