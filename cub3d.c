/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/04/23 12:28:21 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"

bool	g_updated = false;
bool	g_first = false;
int		gun_frame = 0;
float	look = 0.5;
int		mouse_x = 0;
int		mouse_y = 0;
float		mouse_speed = 1;

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

int	set_color_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
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

bool	is_collision(double x, double y, t_game *game)
{
	t_vect grid;

	grid.x = (int)floor(x);
	grid.y = (int)floor(y);
	if (game->map[grid.y][grid.x] == wall)
		return (true);
	return (false);
}

void	move_player(t_game *game)
{
	t_vector move;
	double	move_speed;

	if (game->player.key.rot_l == true)
		game->player.angle += game->player.turn_dir * ROT_SPEED * mouse_speed;
	if (game->player.key.rot_r == true)
		game->player.angle += game->player.turn_dir * ROT_SPEED * mouse_speed;
	move_speed = MOVE_SPEED;
	if (game->player.key.speed == true)
		move_speed += MOVE_SPEED;
	if (game->player.key.key_up == true || game->player.key.key_down == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle) * game->player.walk_dir * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle) * game->player.walk_dir * move_speed;
		if (!is_exceeded((int)floor(move.x), (int)floor(game->player.pos.y), game->player) && game->map[(int)floor(game->player.pos.y)][(int)floor(move.x)] != wall)
			game->player.pos.x = move.x;
		if (!is_exceeded((int)floor(game->player.pos.x), (int)floor(move.y), game->player) && game->map[(int)floor(move.y)][(int)floor(game->player.pos.x)] != wall)
			game->player.pos.y = move.y;
	}
	if (game->player.key.key_left == true || game->player.key.key_right == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle + 0.5 * PI) * game->player.walk_dir * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle + 0.5 * PI) * game->player.walk_dir * move_speed;
		if (!is_exceeded((int)floor(move.x), (int)floor(game->player.pos.y), game->player) && game->map[(int)floor(game->player.pos.y)][(int)floor(move.x)] != wall)
			game->player.pos.x = move.x;
		if (!is_exceeded((int)floor(game->player.pos.x), (int)floor(move.y), game->player) && game->map[(int)floor(move.y)][(int)floor(game->player.pos.x)] != wall)
			game->player.pos.y = move.y;
	}
	if (game->player.key.look_up == true)
	{
		look += (float)(ROT_SPEED) * mouse_speed;
		if (look > 1)
			look = 1;
	}
	if (game->player.key.look_down == true)
	{
		look -= (float)(ROT_SPEED) * mouse_speed;
		if (look < 0)
			look = 0;
	}
	game->player.key.rot_l = false;
	game->player.key.rot_r = false;
	game->player.key.look_up = false;
	game->player.key.look_down = false;
}

void	clear_image(t_image *img, int width, int height, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

int	init_image(t_game *game, t_image *image, int w, int h)
{
	image->img_ptr = mlx_new_image(game->var.mlx, w, h);
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
	if (keysym == XK_g)
		game->player.key.recharge = true;
	if (keysym == XK_Up)
		game->player.key.look_up = true;
	if (keysym == XK_Down)
		game->player.key.look_down = true;
	if (keysym == XK_Alt_L)
	{
		if (game->player.key.hide_mouse == false)
		{
			game->player.key.hide_mouse = true;
			mlx_mouse_hide(game->var.mlx, game->var.win);
		}
		else
		{
			game->player.key.hide_mouse = false;
			mlx_mouse_show(game->var.mlx, game->var.win);
		}
	}
	mouse_speed = 1;
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
	if (keysym == XK_Up)
		game->player.key.look_up = false;
	if (keysym == XK_Down)
		game->player.key.look_down = false;
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
	// put_pixel(img, p.x, p.y, color);
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
}

double	distance_between_points(t_vector a, t_vector b)
{
	return (sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)));
}

void	draw_rectangle_rgb_gradient(t_image *image, int	x0, int x1, int y0, int y1, t_rgb rgb)
{
	int	i;
	int	round;

	round = 0;
	while (y0 < y1)
	{
		i = 0;
		while (i < x1)
		{
			put_pixel(image, i + x0, y0, set_color_rgb(rgb.r, rgb.g, rgb.b));
			i++;
		}
		round++;
		if (round == 2)
		{
			rgb.r -= 1;
			rgb.g -= 1;
			rgb.b -= 1;
			if (rgb.r < 0)
				rgb.r = 0;
			if (rgb.g < 0)
				rgb.g = 0;
			if (rgb.b < 0)
				rgb.b = 0;
			round = 0;
		}
		y0++;
	}
}

void	draw_rectangle_rgb_rgradient(t_image *image, int x0, int x1, int y0, int y1, t_rgb rgb)
{
	int	i;
	int	round;

	round = 0;
	while (y1 > y0)
	{
		i = 0;
		while (i < x1)
		{
			put_pixel(image, i + x0, y1, set_color_rgb(rgb.r, rgb.g, rgb.b));
			i++;
		}
		round++;
		if (round == 3)
		{
			rgb.r -= 1;
			rgb.g -= 1;
			rgb.b -= 1;
			if (rgb.r < 0)
				rgb.r = 0;
			if (rgb.g < 0)
				rgb.g = 0;
			if (rgb.b < 0)
				rgb.b = 0;
			round = 0;
		}
		y1--;
	}
}

void	draw_rectangle(t_image *image, int	x0, int x1, int y0, int y1, int color)
{
	int	i;

	while (y0 < y1)
	{
		i = 0;
		while (i < x1)
		{
			put_pixel(image, i + x0, y0, color);
			i++;
		}
		y0++;
	}
}

void	draw_circle_full(t_image *img, t_vector p, int radius, int color)
{
	int	i;
	int	j;
	int	dist_squared;

	j = -radius;
	while (j <= radius)
	{
		i = -radius;
		while (i <= radius)
		{
			dist_squared = (i * i) + (j * j);
			if ( dist_squared <= (radius * radius))
				put_pixel(img, p.x + i, p.y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_square_at_edge(t_image *img, int x, int y, int size, t_vector c, int color)
{
	int	j;
	int	i;
	int	px;
	int	py;
	double	dist;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size)
		{
			px = i + x;
			py = j + y;
			dist = (px - c.x) * (px - c.x) + (py - c.y) * (py - c.y);
			if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
				put_pixel(img, px, py, color);
			i++;
		}
		j++;
	}
}

void	draw_rotated_square(t_image *img, t_vect s_p, t_vector c, int size, double angle, int color)
{
	int	half;
	int	x;
	int	y;
	t_vect draw;
	t_vector rot;
	double dist;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			rot.x = x * cos(angle) + y * sin(angle);
			rot.y = -x * sin(angle) + y * cos(angle);
			draw.x = (int)(s_p.x + rot.x);
			draw.y = (int)(s_p.y + rot.y);
			dist = (draw.x - c.x) * (draw.x - c.x) + (draw.y - c.y) * (draw.y - c.y);
			if (dist <= (MINI_WIDTH / 2) * (MINI_WIDTH / 2))
				put_pixel(img, draw.x, draw.y, color);
			x++;
		}
		y++;
	}
}

int	get_img_color(t_image *img, int x, int y)
{
	return (*(int *)(img->addr + ((y * img->size_line) + (x * (img->bpp
						/ 8)))));
}

void	paint_image_to_win(t_image *img, t_image *img1, int s_x, int s_y, t_vect p, int color)
{
	int		x;
	int		y;
	int		img_color;

	y = 0;
	while (y < s_y)
	{
		x = 0;
		while (x < s_x)
		{
			int src_x = (int)(double)x * ((double)img1->w / (double)s_x);
			int src_y = (int)(double)y * ((double)img1->h / (double)s_y);
			img_color = get_img_color(img1, src_x, src_y);
			if (img_color >= 0)
			{
				if (color >= 0)
					img_color = color;
				put_pixel(img, x + p.x, y + p.y, img_color);
			}
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_image *img, char **map, t_player player)
{
	int	x1;
	int	x2;
	int	y1;
	int	y2;
	int	side = 7;
	double angle;
	
	angle = normalize_angle(player.angle + (0.5 * PI));
	x1 = (int)floor(player.pos.x) - side;
	if (x1 < 0)
		x1 = 0;
	x2 = player.pos.x + side;
	if (x2 > player.w)
		x2 = player.w;
	y1 = (int)floor(player.pos.y) - side;
	if (y1 < 0)
		y1 = 0;
	y2 = player.pos.y + side;
	if (y2 > player.h)
		y2 = player.h;
	t_vector c = {100, HEIGHT - 100};
	draw_circle_full(img, c, MINI_WIDTH / 2, 0x1E1E1E);
	draw_circle(img, c, MINI_WIDTH / 2, 0x333333);
	draw_circle(img, c, MINI_WIDTH / 2 + 1, 0x555555);
	draw_circle(img, c, MINI_WIDTH / 2 + 2, 0x777777);
	draw_circle(img, c, MINI_WIDTH / 2 + 3, 0x999999);
	double cos_angle = cos(angle);
    double sin_angle = sin(angle);
	int	j = y1;
	int	i;
	while (j < y2)
	{
		i = x1;
		while (i < x2)
		{
			if (map[j][i] == wall)
			{
				t_vector relative = {(i - player.pos.x), (j - player.pos.y)};
				t_vector rotated = {relative.x * cos_angle + relative.y * sin_angle, -relative.x * sin_angle + relative.y * cos_angle};
				t_vect draw = {(int)(c.x + (rotated.x * MINI_SIZE)), (int)(c.y + (rotated.y * MINI_SIZE))};
				// draw_square_at_edge(img, draw.x, draw.y, MINI_SIZE, c, 0xE0E0E0);
				draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0xE0E0E0);
			}
			i++;
		}
		j++;
	}
	// draw_circle(img, c, 1, 0x00FF66);
	// draw_square(img, (int)c.x, (int)c.y, 5, 0x00FF66);
}

void	draw_texture(t_image *img, t_image *txt, int x0, int x1, int y0, int y1, int offset, double dist_wall)
{
	int	i;
	int color;
	int	y;
	double scale;
	int	texture_y;
	double	light = 40 / dist_wall;
	t_rgb col;
	int	r;
	int	g;
	int	b;

	scale = (double)txt->h / (y1 - y0);
	y = 0;
	while (y0 < y1)
	{
		i = 0;
		texture_y = (int)(y * scale);
		while (i < x1)
		{
			color = get_img_color(txt, offset, texture_y);
			r = (color >> 16) & 0xFF;
			g = (color >> 8) & 0xFF;
			b = color & 0xFF;
			col.r = r * light;
			if (col.r >= r)
				col.r = r;
			else if (col.r < 0)
				col.r = 0;
			col.g = g * light;
			if (col.g >= g)
				col.g = g;
			else if (col.r < 0)
				col.g = 0;
			col.b = b * light;
			if (col.b >= b)
				col.b = b;
			else if (col.b < 0)
				col.b = 0;
			put_pixel(img, i + x0, y0, set_color_rgb(col.r, col.g, col.b));
			i++;
		}
		y++;
		y0++;	
	}
}

void	draw_texture_for_floor(t_image *image,  t_image *txt, int x0, int x1, int y0, int y1, int offset)
{
	int	i;
	// int	round;
	t_rgb rgb;
	double scale;
	int	color;
	int	texture_y;
	int	y;

	// round = 0;
	scale = (double)txt->h / (y1 - y0);
	y = 0;
	while (y1 > y0)
	{
		i = 0;
		texture_y = (int)(y * scale);
		while (i < x1)
		{
			color = get_img_color(txt, offset, texture_y);	
			put_pixel(image, i + x0, y1, color);
			i++;
		}
		// round++;
		// if (round == 3)
		// {
		// 	rgb.r -= 1;
		// 	rgb.g -= 1;
		// 	rgb.b -= 1;
		// 	if (rgb.r < 0)
		// 		rgb.r = 0;
		// 	if (rgb.g < 0)
		// 		rgb.g = 0;
		// 	if (rgb.b < 0)
		// 		rgb.b = 0;
		// 	round = 0;
		// }
		y++;
		y1--;
	}
}

void	draw_floor_and_ceil(t_game *game, t_image *texture, int ray, double ray_angle, int start, int end)
{
	double center;
	t_vector p;
	int		color;
	double	cos_ray;
	double	sin_ray;
	double	distance;
	
	p.x = (double)game->player.pos.x * TILE_SIZE;
	p.y = (double)game->player.pos.y * TILE_SIZE;
	cos_ray = cos(ray_angle);
	sin_ray = sin(ray_angle);
	while (start < end)
	{
		if (start > HALF_HEIGHT)
			distance = (510 * (TILE_SIZE / (start - (double)HALF_HEIGHT)));
		else
			distance = (510 * (TILE_SIZE / ((double)HALF_HEIGHT - start)));
		t_vector f = {
			.x = distance * cos_ray + p.x,
			.y = distance * sin_ray + p.y
		};
		t_vector cell = {
			.x = f.x / TILE_SIZE,
			.y = f.y / TILE_SIZE
		};
		if (cell.x >= 0 || cell.y >= 0 || cell.x < game->player.w || cell.y < game->player.h)
		{
			
			int tex_y = (fmod(f.y, TILE_SIZE) / TILE_SIZE) * texture->h;
			int tex_x = (fmod(f.x, TILE_SIZE) / TILE_SIZE) * texture->w;
			if (tex_x >= texture->w)
				tex_x = texture->w - 1;
			else if (tex_x < 0)
				tex_x = 0;
			if (tex_y >= texture->h)
				tex_y = texture->h - 1;
			else if (tex_y < 0)
				tex_y = 0;
			color = get_img_color(texture, tex_x, tex_y);
			put_pixel(&game->img, ray, start, color);
		}
		start++;
	}
}

int	game_loop(t_game *game)
{
	t_vector	a;
	t_vector	b;
	bool	is_facing_down;
	bool	is_facing_left;
	bool	is_facing_right;
	bool	is_facing_up;
	
	if (g_updated == false && g_first == false)
		return (1);
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT, 0);
	move_player(game);
	double	ray_angle;
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
		while (next_horiz.x < (game->player.w * TILE_SIZE) && next_horiz.x >= 0 && next_horiz.y < (game->player.h * TILE_SIZE) && next_horiz.y >= 0)
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
		while (next_vert.x < (game->player.w * TILE_SIZE)  && next_vert.x >= 0 && next_vert.y < (game->player.h * TILE_SIZE)  && next_vert.y >= 0)
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
		double	o;
		if (horizontal_dist < vertical_dist)
		{
			wall_hit = horiz_p;
			dist_wall = fabs(horizontal_dist);
			o = ((fmod(wall_hit.x, TILE_SIZE) / TILE_SIZE));
		}
		else
		{
			wall_hit = vert_p;
			dist_wall = fabs(vertical_dist);
			o = ((fmod(wall_hit.y, TILE_SIZE) / TILE_SIZE));
		}
		dist_wall *= cos(game->player.angle - ray_angle);
		if (dist_wall < 0.001)
			dist_wall = 0.001;
		int	offset;
		t_image *img;
		if (is_facing_up && horizontal_dist < vertical_dist)
		{
			img = &game->pack.no_img; 
			offset = (o * img->w);
		}
		else if (is_facing_down && horizontal_dist < vertical_dist)
		{
			img = &game->pack.so_img; 
			offset = (o * img->w);
		}
		else if (is_facing_right && horizontal_dist > vertical_dist)
		{
			img = &game->pack.ea_img; 
			offset = (o * img->w);
		}
		else if (is_facing_left && horizontal_dist > vertical_dist)
		{
			img = &game->pack.we_img; 
			offset = (o * img->w);			
		}
		double waves = 4 + (cos (MOVE_SPEED + 8 * (game->player.pos.x + game->player.pos.y)));
		float	line_height = (TILE_SIZE / dist_wall) * 510;
		if (line_height > HEIGHT)
			line_height = HEIGHT;
		float	draw_begin = look * (HEIGHT - line_height);
		if (draw_begin <= 0)
			draw_begin = 0;
		else
			draw_begin += waves;
		float	draw_end = draw_begin + line_height + waves;
		if (draw_end >= HEIGHT)
			draw_end = HEIGHT - 1;
		draw_rectangle_rgb_gradient(&game->img, ray * RES, RES, 0, (int)draw_begin, game->txt.c);
		// draw_rectangle_rgb_rgradient(&game->img, ray * RES, RES, (int)draw_end, HEIGHT, game->txt.f);
		draw_texture(&game->img, img, ray * RES, RES, (int)(draw_begin), (int)(draw_end), offset, dist_wall);
		draw_floor_and_ceil(game, &game->pack.ceil, ray, ray_angle, 0, (int)draw_begin);
		draw_floor_and_ceil(game, &game->pack.floor, ray, ray_angle, (int)draw_end, HEIGHT);
		ray_angle += FOV / R_NB;
		ray++;
	}
	t_vect center = {(int)(WIDTH / 2), (int)(HEIGHT / 2)};
	paint_image_to_win(&game->img, &game->pack.aiming, 32, 30, center, 0xFF0000);
	// draw_circle(&game->img, center, 10, 0xFF0000);
	t_vector compass = {WIDTH - 100, 100};
	t_vector needle = {compass.x + cos(game->player.angle) * 40, compass.y + sin(game->player.angle) * 40};
	draw_circle(&game->img, compass, 50, 0xFFFFFF);
	draw_line(&game->img, compass, needle, 0xFFFFFF);
	draw_minimap(&game->img, game->map, game->player);
	t_vect p = {100 - 6, HEIGHT - 106};
	paint_image_to_win(&game->img, &game->pack.arrow, 12, 12, p, 0x00FF00);
	t_vect g = {(int)(WIDTH / 2) - 100, HEIGHT  - 300};
	if (game->player.key.recharge == true)
	{
		g_updated = true;
		if (gun_frame < 15)
		{
			paint_image_to_win(&game->img, &game->pack.gun_sp[gun_frame], 500, 500, g, -1);
			usleep(80000);
			gun_frame++;
		}
		else
		{
			game->player.key.recharge = false;
			gun_frame = 0;
			g_updated = false;
		}
	}
	else
	{
		g_global()->music_pid = false;
		paint_image_to_win(&game->img, &game->pack.gun_sp[0], 500, 500, g, -1);
		g_updated = false;
	}
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	mlx_string_put(game->var.mlx, game->var.win, compass.x - 10, compass.y - 60, 0xFFFFFF, "NORD");
	mlx_string_put(game->var.mlx, game->var.win, compass.x - 10, compass.y + 70, 0xFFFFFF, "SOUTH");
	mlx_string_put(game->var.mlx, game->var.win, compass.x + 60, compass.y + 5, 0xFFFFFF, "EAST");
	mlx_string_put(game->var.mlx, game->var.win, compass.x - 80, compass.y + 5, 0xFFFFFF, "WEST");
	if (game->player.key.hide_mouse == true)
		mlx_string_put(game->var.mlx, game->var.win, 50, 25, 0xFFFFFF, "mouse activate");
	g_first = false;
	return (0);
}

int	ft_mouse_move(int x, int y, t_game *game)
{
	if (game->player.key.hide_mouse == false)
		return (1);
	if ((WIDTH / 2) + 10 < x)
	{
		game->player.key.rot_r = true;
		game->player.turn_dir = 1;
	}
	else if ((WIDTH / 2) - 10 > x)
	{
		game->player.key.rot_l = true;
		game->player.turn_dir = -1;
	}
	if ((HEIGHT / 2) + 10 < y)
		game->player.key.look_down = true;
	else if ((HEIGHT / 2) - 10 > y)
		game->player.key.look_up = true;
	mouse_speed = 0.8;
	mlx_mouse_move(game->var.mlx, game->var.win, WIDTH / 2, HEIGHT / 2);
	g_updated = true;
	return (0);
}

int	ft_mouse_pressed(int button, int x, int y, t_game *game)
{
	if (game->player.key.hide_mouse == false && g_global()->one_shoot_music == true)
		return (1);
	if (button == 1)
	{
		launch_music("sounds/gun1.wav", "2");
		g_global()->one_shoot_music = true;
		game->player.key.recharge = true;
	}
	return (0);
}

int	ft_mouse_release(int button, int x, int y, t_game *game)
{
	if (game->player.key.hide_mouse == false)
		return (1);
	if (button == 1)
		game->player.key.recharge = false;
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
	if (init_image(&game, &game.img, WIDTH, HEIGHT))
	{
		ft_free_txt(&game.txt);
		leave_windows(&game);
	}
	
	game.pack.no_img.img_ptr = mlx_xpm_file_to_image(game.var.mlx, game.txt.no_path, &game.pack.no_img.w, &game.pack.no_img.h);
	game.pack.no_img.addr = mlx_get_data_addr(game.pack.no_img.img_ptr, &game.pack.no_img.bpp, &game.pack.no_img.size_line, &game.pack.no_img.endian);
	game.pack.so_img.img_ptr = mlx_xpm_file_to_image(game.var.mlx, game.txt.so_path, &game.pack.so_img.w, &game.pack.so_img.h);
	game.pack.so_img.addr = mlx_get_data_addr(game.pack.so_img.img_ptr, &game.pack.so_img.bpp, &game.pack.so_img.size_line, &game.pack.so_img.endian);
	game.pack.ea_img.img_ptr = mlx_xpm_file_to_image(game.var.mlx, game.txt.ea_path, &game.pack.ea_img.w, &game.pack.ea_img.h);
	game.pack.ea_img.addr = mlx_get_data_addr(game.pack.ea_img.img_ptr, &game.pack.ea_img.bpp, &game.pack.ea_img.size_line, &game.pack.ea_img.endian);
	game.pack.we_img.img_ptr = mlx_xpm_file_to_image(game.var.mlx, game.txt.we_path, &game.pack.we_img.w, &game.pack.we_img.h);
	game.pack.we_img.addr = mlx_get_data_addr(game.pack.we_img.img_ptr, &game.pack.we_img.bpp, &game.pack.we_img.size_line, &game.pack.we_img.endian);
	game.pack.arrow.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/arrow.xpm", &game.pack.arrow.w, &game.pack.arrow.h);
	game.pack.arrow.addr = mlx_get_data_addr(game.pack.arrow.img_ptr, &game.pack.arrow.bpp, &game.pack.arrow.size_line, &game.pack.arrow.endian);
	game.pack.aiming.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/aiming1.xpm", &game.pack.aiming.w, &game.pack.aiming.h);
	game.pack.aiming.addr = mlx_get_data_addr(game.pack.aiming.img_ptr, &game.pack.aiming.bpp, &game.pack.aiming.size_line, &game.pack.aiming.endian);
	game.pack.floor.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/floor.xpm", &game.pack.floor.w, &game.pack.floor.h);
	game.pack.floor.addr = mlx_get_data_addr(game.pack.floor.img_ptr, &game.pack.floor.bpp, &game.pack.floor.size_line, &game.pack.floor.endian);
	game.pack.ceil.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/ceil.xpm", &game.pack.ceil.w, &game.pack.ceil.h);
	game.pack.ceil.addr = mlx_get_data_addr(game.pack.ceil.img_ptr, &game.pack.ceil.bpp, &game.pack.ceil.size_line, &game.pack.ceil.endian);


	int	i = 0;
	char file[] = "images/gun/00.xpm";
	while (i < 15)
	{
		if (i <= 8)
			file[12] = (i + 1) + '0';
		else
		{
			file[12] = ((i + 1) % 10) + '0';
			file[11] = ((i + 1) / 10) + '0';
		}
		game.pack.gun_sp[i].img_ptr = mlx_xpm_file_to_image(game.var.mlx, file, &game.pack.gun_sp[i].w, &game.pack.gun_sp[i].h);
		game.pack.gun_sp[i].addr = mlx_get_data_addr(game.pack.gun_sp[i].img_ptr, &game.pack.gun_sp[i].bpp, &game.pack.gun_sp[i].size_line, &game.pack.gun_sp[i].endian);
		i++;
	}
	mlx_put_image_to_window(game.var.mlx, game.var.win, game.img.img_ptr, 0,
		0);
	// // mlx_string_put(game.var.mlx, game.var.win, WIDTH / 2, HEIGHT - TILE_SIZE,
	// // 0xFF0000, "PRESS THE START TO BEGIN");
	g_updated = false;
	g_first = true;
	mlx_mouse_get_pos(game.var.mlx, game.var.win, &mouse_x, &mouse_y);
	mlx_hook(game.var.win, DestroyNotify, 0, leave_windows, &game);
	mlx_hook(game.var.win, KeyPress, KeyPressMask, ft_key_press, &game);
	mlx_hook(game.var.win, KeyRelease, KeyReleaseMask, ft_key_release, &game);
	mlx_hook(game.var.win, ButtonPress, ButtonPressMask, ft_mouse_pressed, &game);
	mlx_hook(game.var.win, MotionNotify, PointerMotionMask, ft_mouse_move, &game);
	mlx_loop_hook(game.var.mlx, game_loop, &game);
	mlx_loop(game.var.mlx);
	return (0);
}