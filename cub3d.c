/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/04/28 14:30:38 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/cub3d.h"

bool	g_updated = false;
bool	g_first = false;
int		gun_frame = 0;
float	look = 0;
int		mouse_x = 0;
int		mouse_y = 0;
float		mouse_speed = 1;
int		jump_scale = 30;

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

bool is_exceeded(t_vector move, t_player player)
{
	t_vect top_left;
	t_vect top_right;
	t_vect bottom_left;
	t_vect bottom_right;
	double	collider = 0.1;

	top_left.x = (int)(move.x - collider);
	top_left.y = (int)(move.y - collider);
	top_right.x = (int)(move.x + collider);
	top_right.y = (int)(move.y - collider);
	bottom_left.x = (int)(move.x - collider);
	bottom_left.y = (int)(move.y + collider);
	bottom_right.x = (int)(move.x + collider);
	bottom_right.y = (int)(move.y + collider);
	if (top_left.x >= player.w || top_left.x < 0 || top_left.y < 0 || top_left.y >= player.h)
		return (true);
	if (top_right.x >= player.w || top_right.x < 0 || top_right.y < 0 || top_right.y >= player.h)
		return (true);
	if (bottom_left.x >= player.w || bottom_left.x < 0 || bottom_left.y < 0 || bottom_left.y >= player.h)
		return (true);
	if (bottom_right.x >= player.w || bottom_right.x < 0 || bottom_right.y < 0 || bottom_right.y >= player.h)
		return (true);
	return(false);
}

bool	is_hit_wall(int tile_x, int tile_y, char **map)
{
	if (map[tile_y][tile_x] == wall || map[tile_y][tile_x] == door)
		return (true);
	return (false);	
}

bool	is_collision(t_vector move, t_player player, char **map)
{
	t_vect top_left;
	t_vect top_right;
	t_vect bottom_left;
	t_vect bottom_right;
	double	collider = 0.1;

	top_left.x = (int)(move.x - collider);
	top_left.y = (int)(move.y - collider);
	top_right.x = (int)(move.x + collider);
	top_right.y = (int)(move.y - collider);
	bottom_left.x = (int)(move.x - collider);
	bottom_left.y = (int)(move.y + collider);
	bottom_right.x = (int)(move.x + collider);
	bottom_right.y = (int)(move.y + collider);
	if (is_hit_wall(top_left.x, top_left.y, map) ||
		is_hit_wall(top_right.x, top_right.y, map) ||
		is_hit_wall(bottom_left.x, bottom_left.y, map) ||
		is_hit_wall(bottom_right.x, bottom_right.y, map))
		return (true);
	return (false);
}

void	move_player(t_game *game)
{
	t_vector move;
	double	move_speed;
	t_vector	player;

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
		if (!is_exceeded(move, game->player) && !is_collision(move, game->player, game->map))
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
		else
		{
			if (!is_exceeded((t_vector){move.x, game->player.pos.y}, game->player) && !is_collision((t_vector){move.x, game->player.pos.y}, game->player, game->map))
				game->player.pos.x = move.x;
			if (!is_exceeded((t_vector){game->player.pos.x, move.y}, game->player) && !is_collision((t_vector){game->player.pos.x, move.y}, game->player, game->map))
				game->player.pos.y = move.y;
		}
	}
	if (game->player.key.key_left == true || game->player.key.key_right == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle + 0.5 * PI) * game->player.walk_dir * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle + 0.5 * PI) * game->player.walk_dir * move_speed;
		if (!is_exceeded(move, game->player) && !is_collision(move, game->player, game->map))
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
		else
		{
			if (!is_exceeded((t_vector){move.x, game->player.pos.y}, game->player) && !is_collision((t_vector){move.x, game->player.pos.y}, game->player, game->map))
				game->player.pos.x = move.x;
			if (!is_exceeded((t_vector){game->player.pos.x, move.y}, game->player) && !is_collision((t_vector){game->player.pos.x, move.y}, game->player, game->map))
				game->player.pos.y = move.y;
		}
	}
	if (game->player.key.look_up == true)
	{
		look += (float)(ROT_SPEED) * mouse_speed;
		if (look >= 0.3)
			look = 0.3;
	}
	if (game->player.key.look_down == true)
	{
		look -= (float)(ROT_SPEED) * mouse_speed;
		if (look <= - 0.3)
			look = - 0.3;
	}
	if (game->player.key.key_croutch == true)
	{
		if (g_global()->j_croutch == 0)
			g_global()->j_croutch = -200;
		else
			g_global()->j_croutch = 0;
	}
	if (game->player.key.key_jump == true)
	{
		if (jump_scale > 0 && g_global()->j_croutch < 500)
		{
			g_global()->j_croutch += jump_scale;
			look -= 0.03;
			if (g_global()->j_croutch >= 500)
				jump_scale *= -1;
		}
		else if (jump_scale < 0)
		{
			g_global()->j_croutch += jump_scale;
			look += 0.03;
			if (g_global()->j_croutch <= 0)
			{
				g_global()->j_croutch = 0;
				jump_scale = 30;
				g_updated = false;
				game->player.key.key_jump = false;
				look = 0.5;
			}
			
		}
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
	if (keysym == XK_c)
		game->player.key.key_croutch = true;
	if (keysym == XK_space && game->player.key.key_jump == false)
		game->player.key.key_jump = true;
	if (keysym == XK_i)
	{
		g_global()->zoom += 0.1;
		g_global()->narrow_look += 100;
	}
	if (keysym == XK_o)
	{
		g_global()->zoom -= 0.1;
		g_global()->narrow_look -= 100;
	}
	if (g_global()->zoom < 1)
	{
		g_global()->zoom = 1;
		g_global()->narrow_look = 875;
	}
	if (g_global()->zoom > 3.0)
	{
		g_global()->zoom = 3.0;
		g_global()->narrow_look = 2875;
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
	if (keysym == XK_c)
		game->player.key.key_croutch = false;
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

bool	is_wall(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == wall || map[y1][x1] == door)
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
			t_vector relative = {(i - player.pos.x), (j - player.pos.y)};
			t_vector rotated = {relative.x * cos_angle + relative.y * sin_angle, -relative.x * sin_angle + relative.y * cos_angle};
			t_vect draw = {(int)(c.x + (rotated.x * MINI_SIZE)), (int)(c.y + (rotated.y * MINI_SIZE))};
			if (map[j][i] == wall)
				draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0xE0E0E0);
			else if (map[j][i] == door)
				draw_rotated_square(img, draw, c, MINI_SIZE - 1, angle, 0x82490b);
			// draw_square_at_edge(img, draw.x, draw.y, MINI_SIZE, c, 0xE0E0E0);
			
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
	double scale;
	int	texture_y;
	double	light = 40 / dist_wall;
	t_rgb col;
	int	r;
	int	g;
	int	b;
	double	tex_pos = 0;

	scale = (double)txt->h / (double)(y1 - y0);
	while (y0 <= y1)
	{
		i = 0;
		texture_y = (int)tex_pos;
		if (texture_y >= txt->h)
			texture_y = txt->h - 1;
		while (i < x1)
		{
			color = get_img_color(txt, offset, texture_y);
			if (color >= 0)
			{
				r = (color >> 16) & 0xFF;
				g = (color >> 8) & 0xFF;
				b = color & 0xFF;
				col.r = fmin(fmax(r * light, 0), r);
				col.g = fmin(fmax(g * light, 0), g);
				col.b = fmin(fmax(b * light, 0), b);
				put_pixel(img, i + x0, y0, set_color_rgb(col.r, col.g, col.b));
			}
			i++;
		}
		tex_pos += scale;
		y0++;	
	}
}

void	draw_floor_and_ceil(t_game *game, t_image *texture, int ray, double ray_angle, int start, int end, double middle, bool floor)
{
	double center;
	t_vector p;
	int		color;
	double	cos_ray;
	double	sin_ray;
	double	distance;
	
	p.x = (double)game->player.pos.x * TILE_SIZE;
	p.y = (double)game->player.pos.y * TILE_SIZE;
	double	tile_size_inv = 1.0 / TILE_SIZE;
	cos_ray = cos(ray_angle);
	sin_ray = sin(ray_angle);
	double	beta = fabs(ray_angle - game->player.angle);
	while (start < end)
	{
		double r = start - middle;
		double s = TILE_SIZE * g_global()->project_dist / r;
		double distance = s / cos(beta);
		t_vector f;
		if (floor == false)
		{
			f.x = p.x - cos_ray * distance;
			f.y = p.y - sin_ray * distance;
		}
		else
		{
			f.x = p.x + cos_ray * distance;
			f.y = p.y + sin_ray * distance;
		}
		int tex_x = (int)((fmod(f.x , TILE_SIZE) * tile_size_inv) * texture->w);
		int tex_y = (int)((fmod(f.y, TILE_SIZE) * tile_size_inv) * texture->h);
		tex_x = abs(tex_x);
		tex_y = abs(tex_y);
		color = get_img_color(texture, tex_x, tex_y);
		put_pixel(&game->img, ray, start, color);
		start++;
	}
}

void	draw_rotated_texture(t_image *dest, t_image *src, t_vector center, double angle, int s_x, int s_y)
{
	t_vect c;
	int		y;
	int		x;
	int		color;
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle + (0.5 * PI));
	sin_a = sin(angle + (0.5 * PI));
	y = 0;
	while (y < s_y)
	{
		x = 0;
		while (x < s_y)
		{
			int src_x = (int)(double)x * ((double)src->w / (double)s_x);
			int src_y = (int)(double)y * ((double)src->h / (double)s_y);
			color = get_img_color(src, src_x, src_y);
			if (color >= 0)
			{
				t_vect d = {
					.x = x - (s_x / 2),
					.y = y - (s_y / 2)
				};
				t_vector r = {
					.x = cos_a * d.x - sin_a * d.y,
					.y = sin_a * d.x + cos_a * d.y
				};
				put_pixel(dest, (int)((center.x + r.x)), (int)(center.y + r.y), color);
			}
			x++;
		}
		y++;
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
	double	ray_angle;
	int		ray = 0;
	double	project_dist;

	if (g_updated == false && g_first == false)
		return (1);
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT, 0);
	move_player(game);
	a.x = game->player.pos.x * TILE_SIZE;
	a.y = game->player.pos.y * TILE_SIZE;
	double	adjusted_fov = FOV / g_global()->zoom;
	g_global()->project_dist = (800 / tan(adjusted_fov / 2)) - g_global()->narrow_look;
	ray_angle = normalize_angle(game->player.angle - (adjusted_fov / 2));
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
		if (ray_angle < 1.57079632679 || ray_angle > 4.71238898038) // 0.5 * PI || 1.5 * PI
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
		img = NULL;
		if (game->map[(int)floor(wall_hit.y / TILE_SIZE)][(int)floor(wall_hit.x / TILE_SIZE)] == door)
		{
			if (o < 0.33)
			{
				// dist_wall += 1;
				img = &game->pack.no_img;
			}
			else if (o > 0.66)
			{
				// dist_wall
				img = &game->pack.no_img;
			}
			else
			{
				dist_wall += (TILE_SIZE / 4);
				img = &game->pack.door[0];
			}
		}
		if (is_facing_up && horizontal_dist < vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.no_img; 
			offset = (int)(o * img->w);
		}
		else if (is_facing_down && horizontal_dist < vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.so_img;
			offset = img->w - (int)(o * img->w) - 1;
		}
		else if (is_facing_right && horizontal_dist > vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.ea_img;
			offset = (int)(o * img->w);
		}
		else if (is_facing_left && horizontal_dist > vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.we_img;
			offset = img->w - (int)(o * img->w) - 1;
		}
		double waves = 4 + (cos (MOVE_SPEED + 8 * (game->player.pos.x + game->player.pos.y)));
		double	line_height = (TILE_SIZE / dist_wall) * (g_global()->project_dist);
		double middle = (HEIGHT / 2) + (look * HEIGHT);
		double	draw_begin = middle - (line_height / 2) + g_global()->j_croutch;
		double	draw_end = draw_begin + line_height;
		// draw_rectangle_rgb_gradient(&game->img, ray * RES, RES, 0, (int)draw_begin, game->txt.c);
		// draw_rectangle_rgb_rgradient(&game->img, ray * RES, RES, (int)draw_end, HEIGHT, game->txt.f);
		draw_texture(&game->img, img, ray * RES, RES, (int)(draw_begin), (int)(draw_end), offset, dist_wall);
		// if (draw_begin <= 0)
		// 	draw_begin = 0;
		// if (draw_end >= HEIGHT)
		// 	draw_end = HEIGHT - 1;   posssible necessary
		draw_floor_and_ceil(game, &game->pack.ceil, ray, ray_angle, 0, (int)draw_begin, middle, false);
		draw_floor_and_ceil(game, &game->pack.floor, ray, ray_angle, (int)draw_end + 1, HEIGHT, middle, true);
		ray_angle += adjusted_fov / R_NB;
		ray++;
	}
	t_vect center = {(int)(WIDTH / 2), (int)(HEIGHT / 2)};
	if (game->player.key.scope == true)
		paint_image_to_win(&game->img, &game->pack.scope, WIDTH, HEIGHT, (t_vect){0, 0}, -1);
	else
		paint_image_to_win(&game->img, &game->pack.aiming, 32, 30, center, 0xFF0000);
	t_vector compass = {WIDTH - 124, 95};
	t_vect comp = {WIDTH - 200, 20};
	paint_image_to_win(&game->img, &game->pack.compass, 150, 150, comp, -1);
	draw_rotated_texture(&game->img, &game->pack.needle, compass, game->player.angle, 100, 100);
	draw_minimap(&game->img, game->map, game->player);
	t_vect p = {100 - 7, HEIGHT - 100 - 7};
	paint_image_to_win(&game->img, &game->pack.arrow, 12, 12, p, 0x00FF00);
	// draw_circle_full(&game->img, p, 4, 0x00FF00);
	t_vect g = {800 - 100, (int)(HEIGHT  - 300 - (look * 50))};
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
	else if (game->player.key.key_jump == false && game->player.key.scope == false)
	{
		g_global()->music_pid = false;
		paint_image_to_win(&game->img, &game->pack.gun_sp[0], 500, 500, g, -1);
		g_updated = false;
	}
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	// mlx_string_put(game->var.mlx, game->var.win, compass.x - 10, compass.y - 60, 0xFFFFFF, "NORD");
	// mlx_string_put(game->var.mlx, game->var.win, compass.x - 10, compass.y + 70, 0xFFFFFF, "SOUTH");
	// mlx_string_put(game->var.mlx, game->var.win, compass.x + 60, compass.y + 5, 0xFFFFFF, "EAST");
	// mlx_string_put(game->var.mlx, game->var.win, compass.x - 80, compass.y + 5, 0xFFFFFF, "WEST");
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
		g_updated = true;
	}
	else if ((WIDTH / 2) - 10 > x)
	{
		game->player.key.rot_l = true;
		game->player.turn_dir = -1;
		g_updated = true;
	}
	if ((HEIGHT / 2) + 10 < y)
	{
		game->player.key.look_down = true;
		g_updated = true;
	}
	else if ((HEIGHT / 2) - 10 > y)
	{
		game->player.key.look_up = true;
		g_updated = true;
	}
	mouse_speed = 0.8;
	mlx_mouse_move(game->var.mlx, game->var.win, WIDTH / 2, HEIGHT / 2);
	return (0);
}

int	ft_mouse_pressed(int button, int x, int y, t_game *game)
{
	if (game->player.key.hide_mouse == false && g_global()->one_shoot_music == true)
		return (1);
	if (button == 1 && game->player.key.hide_mouse == true)
	{
		// launch_music("sounds/gun1.wav", "2");
		g_global()->one_shoot_music = true;
		game->player.key.recharge = true;
		g_updated = true;
	}
	if (button == 4 && game->player.key.hide_mouse == true)
	{
		game->player.key.scope = true;
		g_global()->zoom += 0.1;
		g_global()->narrow_look += 100;
		g_updated = true;
	}
	if (button == 5 && game->player.key.hide_mouse == true)
	{
		g_global()->zoom -= 0.1;
		g_global()->narrow_look -= 100;
		g_updated = true;
	}
	if (g_global()->zoom < 1)
	{
		game->player.key.scope = false;
		g_global()->zoom = 1;
		g_global()->narrow_look = 875;
	}
	if (g_global()->zoom > 3.0)
	{
		g_global()->zoom = 3.0;
		g_global()->narrow_look = 2875;
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
	game.pack.floor.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/floor3.xpm", &game.pack.floor.w, &game.pack.floor.h);
	game.pack.floor.addr = mlx_get_data_addr(game.pack.floor.img_ptr, &game.pack.floor.bpp, &game.pack.floor.size_line, &game.pack.floor.endian);
	game.pack.ceil.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/ceil0.xpm", &game.pack.ceil.w, &game.pack.ceil.h);
	game.pack.ceil.addr = mlx_get_data_addr(game.pack.ceil.img_ptr, &game.pack.ceil.bpp, &game.pack.ceil.size_line, &game.pack.ceil.endian);
	game.pack.door[0].img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/my_door/open0.xpm", &game.pack.door[0].w, &game.pack.door[0].h);
	game.pack.door[0].addr = mlx_get_data_addr(game.pack.door[0].img_ptr, &game.pack.door[0].bpp, &game.pack.door[0].size_line, &game.pack.door[0].endian);


	game.pack.compass.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/compass2.xpm", &game.pack.compass.w, &game.pack.compass.h);
	game.pack.compass.addr = mlx_get_data_addr(game.pack.compass.img_ptr, &game.pack.compass.bpp, &game.pack.compass.size_line, &game.pack.compass.endian);
	game.pack.needle.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/needle.xpm", &game.pack.needle.w, &game.pack.needle.h);
	game.pack.needle.addr = mlx_get_data_addr(game.pack.needle.img_ptr, &game.pack.needle.bpp, &game.pack.needle.size_line, &game.pack.needle.endian);
	game.pack.scope.img_ptr = mlx_xpm_file_to_image(game.var.mlx, "images/scope.xpm", &game.pack.scope.w, &game.pack.scope.h);
	game.pack.scope.addr = mlx_get_data_addr(game.pack.scope.img_ptr, &game.pack.scope.bpp, &game.pack.scope.size_line, &game.pack.scope.endian);


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