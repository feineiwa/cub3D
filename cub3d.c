/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:22:58 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/05 08:24:30 by frahenin         ###   ########.fr       */
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
		return (0);
	game->var.win = mlx_new_window(game->var.mlx, WIDTH, HEIGHT, "Cub3d");
	if (!game->var.win)
	{
		mlx_destroy_display(game->var.mlx);
		ft_free(game->var.mlx);
		return (0);
	}
	return (1);
}

int	leave_windows(t_game *game)
{
	free_all(game);
	stop_music();
	exit(0);
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
	t_vector move;
	double	move_speed;
	t_vector	player;

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
		if (!check_collision(move, game->player, game->map))
		{
			game->player.pos.x = move.x;
			game->player.pos.y = move.y;
		}
		else
		{
			if (!check_collision((t_vector){move.x, game->player.pos.y}, game->player, game->map))
				game->player.pos.x = move.x;
			if (!check_collision((t_vector){game->player.pos.x, move.y}, game->player, game->map))
				game->player.pos.y = move.y;
		}
	}
	if (game->player.key.key_right == true || game->player.key.key_left == true)
	{
		move.x = game->player.pos.x + cos(game->player.angle + 0.5 * PI) * game->player.walk_dir_side * move_speed;
		move.y = game->player.pos.y + sin(game->player.angle + 0.5 * PI) * game->player.walk_dir_side * move_speed;
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
		g_global()->look += (float)(ROT_SPEED);
		if (g_global()->look >= 0.3)
			g_global()->look = 0.3;
	}
	if (game->player.key.look_down == true)
	{
		g_global()->look -= (float)(ROT_SPEED);
		if (g_global()->look <= -0.3)
			g_global()->look = -0.3;
	}
	if (game->player.key.key_croutch == true)
	{
		if (g_global()->j_croutch == 0)
		{
			g_global()->j_croutch = -300;
			g_global()->look += 0.2;
		}
		else
		{
			g_global()->j_croutch = 0;
			g_global()->look = 0;
			game->player.key.key_croutch = false;
		}
	}
	if (game->player.key.key_jump == true)
	{
		if (g_global()->jump > 0 && g_global()->j_croutch < 500)
		{
			g_global()->j_croutch += g_global()->jump;
			g_global()->look -= 0.08;
			if (g_global()->j_croutch >= 500)
				g_global()->jump *= -1;
		}
		else if (g_global()->jump < 0)
		{
			g_global()->j_croutch += g_global()->jump;
			g_global()->look += 0.08;
			if (g_global()->j_croutch <= 0)
			{
				g_global()->j_croutch = 0;
				g_global()->jump = 100;
				g_global()->updated = false;
				game->player.key.key_jump = false;
			}
		}
	}
	game->player.key.rot_l = false;
	game->player.key.rot_r = false;
	game->player.key.look_up = false;
	game->player.key.look_down = false;
}

int	init_image_win(t_game *game, t_image *image, int w, int h)
{
	image->img_ptr = mlx_new_image(game->var.mlx, w, h);
	if (!image->img_ptr)
		return (0);
	image->addr = mlx_get_data_addr(image->img_ptr, &image->bpp,
			&image->size_line, &image->endian);
	return (1);
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
				// col.r = r;
				// col.g = g;
				// col.b = b;
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

void	draw_floor_and_ceil(t_game *game, t_image *texture, int ray, double ray_angle, int start, int end, double middle, bool fl)
{
	t_vector p;
	int		color;
	double	cos_ray;
	double	sin_ray;
	double	distance;
	double r;
	
	p.x = (double)game->player.pos.x * TILE_SIZE;
	p.y = (double)game->player.pos.y * TILE_SIZE;
	double	tile_size_inv = 1.0 / TILE_SIZE;
	cos_ray = cos(ray_angle);
	sin_ray = sin(ray_angle);
	double	beta = normalize_angle(ray_angle - game->player.angle);
	while (start < end)
	{
		if (fl)
			r = start - middle;
		else
			r = middle - start;
		double s = (TILE_SIZE * g_global()->project_dist) / r;
		double distance = s / fabs(cos(beta) + 1.0);
		t_vector f;
		f.x = p.x + cos_ray * distance;
		f.y = p.y + sin_ray * distance;
		int tex_x = (int)((fmod(f.x, TILE_SIZE) / (TILE_SIZE)) * texture->w);
		int tex_y = (int)((fmod(f.y, TILE_SIZE) / (TILE_SIZE)) * texture->h);
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

bool	is_hit_door(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == door_c || map[y1][x1] == door_o)
		return (true);
	return (false);
}

bool	is_hit_door_close(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == door_c)
		return (true);
	return (false);
}

bool	is_hit_door_open(int x, int y, char **map, t_player player)
{
	int	x1;
	int	y1;

	x1 = x / TILE_SIZE;
	y1 = y / TILE_SIZE;
	if (x1 < 0 || x1 > player.w - 1 || y1 < 0 || y1 > player.h - 1)
		return (false);
	if (map[y1][x1] == door_o)
		return (true);
	return (false);
}

void	draw_sprite(t_game *game, t_vector sp_screen, double proj_sp_height, t_image *img, double ray_dist[WIDTH], double dist)
{
	int	sprite_height = (int)proj_sp_height;
	int	sprite_width = sprite_height;
	int	start_y = (int)(sp_screen.y - (sprite_height / 2) * 0.4);
	int end_y = start_y + sprite_height;
	int	start_x = (int)(sp_screen.x - sprite_width / 2);
	int	end_x = start_x + sprite_width;
	int	x = start_x;
	int	y = start_y;
	while (x < end_x)
	{
		if (x >= 0 && x < WIDTH && dist < ray_dist[x])
		{
			int	tex_x = (x - start_x) * img->w / sprite_width;
			y = start_y;
			while (y < end_y)
			{
				if (y >= 0 && y < HEIGHT)
				{
					int tex_y = (y - start_y) * img->h / sprite_height;
					int	color = get_img_color(img, tex_x, tex_y);
					if (color >= 0)
						put_pixel(&game->img, x, y, color);
				}
				y++;
			}
		}
		x++;
	}
}

void	sort_sprites_by_distance(t_sprit *sp)
{
	int		i;
	int		j;
	t_sprit	temp;

	j = 0;
	while (j < g_global()->sp_count - 1)
	{
		i = 0;
		while (i < g_global()->sp_count - j - 1)
		{
			if (sp[i].dist < sp[i + 1].dist)
			{
				temp = sp[i];
				sp[i] = sp[i + 1];
				sp[i + 1] = temp;
			}
			i++;
		}
		j++;
	}
}

int	get_health_color(int health, int max_health)
{
	t_rgb	color;
	float	ratio = (float)health / max_health;
	
	color.r = (int)((1.0f - ratio) * 255);
	color.g = (int)(ratio * 255);
	color.b = 0;
	return(set_color_rgb(color.r, color.g, color.b));
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
	int		dist_door = __INT_MAX__;
	t_vector	door_hit;
	double 	middle;
	double	ray_dist[R_NB];
	double 	waves;
	t_vector	door_o_horiz;
	t_vector	door_o_vert;
	bool	found_h_o_door = false;
	bool	found_v_o_door = false;
	double horizontal_dist = 0;
	double vertical_dist = 0;
	t_vector door_v;
	t_vector door_h;
	
	// if (g_global()->updated == false && g_global()->first == false)
	// 	return (1);
	if (g_global()->first == true)
	{
		// paint_image_to_win(&game->img, &game->intro.img[game->intro.frm], WIDTH, HEIGHT, (t_vect){0, 0}, -1);
		mlx_put_image_to_window(game->var.mlx, game->var.win, game->intro.img[game->intro.frm].img_ptr, 0, 0);
		if (get_current_time() - game->intro.last_time >= 90 && game->intro.frm < 383)
		{
			game->intro.frm++;
			game->intro.last_time = get_current_time();
		}
		return (0);
	}
	if (g_global()->game_over == true)
	{
		paint_image_to_win(&game->img, &game->pack.gov, WIDTH, HEIGHT, (t_vect){0, 0}, -1);
		mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0, 0);
		return (0);
	}
	if (g_global()->help == true)
	{
		paint_image_to_win(&game->img, &game->pack.help, WIDTH, HEIGHT, (t_vect){0, 0}, -1);
		mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0, 0);
		return (0);
	}
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT, 0);
	check_door_close(game);
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
		t_vector s = {0, 0};
		compute_ray_orientation(ray_angle, &is_facing_up, &is_facing_down, &is_facing_left, &is_facing_right);
		t_vector horiz_p = {0, 0};
		t_vector vert_p = {0, 0};
		bool	found_horizontal = false;
		bool	found_vertical = false;
		found_h_o_door = false;
		found_v_o_door = false;
		t_vector first_int = {0, 0};
		horiz_p = ray_cast_horizontal(game , ray_angle, is_facing_up, is_facing_down, &found_h_o_door, &found_horizontal);
		vert_p = ray_cast_vertical(game , ray_angle, is_facing_left, is_facing_right, &found_v_o_door, &found_vertical);
		// // distance calculation
		t_vector wall_hit = {0, 0};
		double		dist_wall = 0;
		int	color = 0;
		if (found_horizontal == true)
			horizontal_dist = distance_between_points(a, horiz_p);
		else
			horizontal_dist = __DBL_MAX__;
		if (found_vertical == true)
			vertical_dist = distance_between_points(a, vert_p);
		else
			vertical_dist = __DBL_MAX__;
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
		int	offset;
		t_image *img;
		img = NULL;
		if (is_facing_up && horizontal_dist < vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.no_img;
			int map_x = (int)floor((wall_hit.x / TILE_SIZE));
			int map_y = (int)floor(wall_hit.y / TILE_SIZE) + 1;
			if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
				img = &game->pack.door[1];
			offset = (int)(o * img->w);
		}
		else if (is_facing_down && horizontal_dist < vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.so_img;
			int map_x = (int)floor((wall_hit.x / TILE_SIZE));
			int map_y = (int)floor(wall_hit.y / TILE_SIZE) - 1;
			if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
				img = &game->pack.door[1];
			offset = img->w - (int)(o * img->w) - 1;
		}
		else if (is_facing_right && horizontal_dist > vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.ea_img;
			int map_x = (int)floor((wall_hit.x / TILE_SIZE) - 1);
			int map_y = (int)floor(wall_hit.y / TILE_SIZE);
			if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
				img = &game->pack.door[1];
			offset = (int)(o * img->w);
		}
		else if (is_facing_left && horizontal_dist > vertical_dist)
		{
			if (img == NULL)
				img = &game->pack.we_img;
			int map_x = (int)floor((wall_hit.x / TILE_SIZE) + 1);
			int map_y = (int)floor(wall_hit.y / TILE_SIZE);
			if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
				img = &game->pack.door[1];
			offset = img->w - (int)(o * img->w) - 1;
		}
		if (is_hit_door((int)floor(wall_hit.x), (int)floor(wall_hit.y), game->map, game->player))
		{
			img = NULL;
			double door_v_dist;
			double	door_h_dist;
			int	door_index = find_door(game->doors, (int)floor(wall_hit.x / TILE_SIZE), (int)floor(wall_hit.y / TILE_SIZE));
			if (horizontal_dist < vertical_dist)
			{
				if (is_facing_up)
					s.y = - TILE_SIZE / 2;
				else if (is_facing_down)
					s.y = TILE_SIZE / 2;
				door_h.y = horiz_p.y + s.y;
				door_h.x = horiz_p.x +  (s.y / tan(ray_angle));
				door_v = vert_p;
				door_h_dist = distance_between_points(a, door_h);
				door_v_dist = vertical_dist;
				if (door_h_dist < door_v_dist)
					o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
				else
					o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
			}
			else
			{
				if (is_facing_left)
					s.x = - TILE_SIZE / 2;
				else if (is_facing_right)
					s.x = TILE_SIZE / 2;
				door_v.x = vert_p.x + s.x;
				door_v.y = vert_p.y + (s.x * tan(ray_angle));
				door_h = horiz_p;
				door_v_dist = distance_between_points(a, door_v);
				door_h_dist = horizontal_dist;
				if (door_h_dist < door_v_dist)
					o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
				else
					o = 1.0 - ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
			}
			if (o <= game->doors[door_index].door_prog)
			{
				t_vector	player;

				player = game->player.pos;
				if (horizontal_dist < vertical_dist)
				{
					found_horizontal = false;
					found_h_o_door = false;
					game->player.pos.x = (horiz_p.x / TILE_SIZE);
					game->player.pos.y = (horiz_p.y / TILE_SIZE);
					horiz_p = ray_cast_horizontal(game, ray_angle, is_facing_up, is_facing_down, &found_h_o_door, &found_horizontal);
					if (found_horizontal == true)
						horizontal_dist = distance_between_points(a, horiz_p);
					else
						horizontal_dist = __DBL_MAX__;
				}
				else
				{
					found_vertical = false;
					found_v_o_door = false;
					game->player.pos.x = (vert_p.x / TILE_SIZE);
					game->player.pos.y = (vert_p.y / TILE_SIZE);
					vert_p = ray_cast_vertical(game, ray_angle, is_facing_left, is_facing_right, &found_v_o_door, &found_vertical);
					if (found_vertical == true)
						vertical_dist = distance_between_points(a, vert_p);
					else
						vertical_dist = __DBL_MAX__;
				}
				game->player.pos = player;
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
				if (is_hit_door((int)floor(wall_hit.x), (int)floor(wall_hit.y), game->map, game->player))
				{
					if (horizontal_dist < vertical_dist)
					{
						if (is_facing_up)
							s.y = - TILE_SIZE / 2;
						else if (is_facing_down)
							s.y = TILE_SIZE / 2;
						door_h.y = horiz_p.y + s.y;
						door_h.x = horiz_p.x +  (s.y / tan(ray_angle));
						door_v = vert_p;
						door_h_dist = distance_between_points(a, door_h);
						door_v_dist = vertical_dist;
						if (door_h_dist < door_v_dist)
						{
							o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
							if (is_facing_up || is_facing_down)
								img = &game->pack.door[0];
							dist_wall = door_h_dist;
							wall_hit = door_h;
							if (is_facing_up)
								offset = (int)(1.0 - o * img->w);
							else if (is_facing_down)
								offset = (int)(1.0 - o * img->w);
						}
						else
						{
							o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
							if (is_facing_left || is_facing_right)
								img = &game->pack.door[1];
							dist_wall = door_v_dist;
							wall_hit = door_v;
							if (is_facing_left)
								offset = (int)(1.0 - o * img->w);
							else if (is_facing_right)
								offset = (int)(o * img->w);
						}
					}
					else
					{
						if (is_facing_left)
							s.x = - TILE_SIZE / 2;
						else if (is_facing_right)
							s.x = TILE_SIZE / 2;
						door_v.x = vert_p.x + s.x;
						door_v.y = vert_p.y + (s.x * tan(ray_angle));
						door_h = horiz_p;
						door_v_dist = distance_between_points(a, door_v);
						door_h_dist = horizontal_dist;
						if (door_h_dist < door_v_dist)
						{
							o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
							if (is_facing_up || is_facing_down)
								img = &game->pack.door[1];
							dist_wall = door_h_dist;
							wall_hit = door_h;
							if (is_facing_up)
								offset = (int)(o * img->w);
							else if (is_facing_down)
								offset = (int)(1.0 - o * img->w);
						}
						else
						{
							o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
							if (is_facing_left || is_facing_right)
								img = &game->pack.door[0];
							dist_wall = door_v_dist;
							wall_hit = door_v;
							if (is_facing_left)
								offset = (int)(o * img->w);
							else if (is_facing_right)
								offset = (int)(o * img->w);
						}
					}
				}
			}
			else
			{
				if (horizontal_dist < vertical_dist)
				{
					if (is_facing_up)
						s.y = - TILE_SIZE / 2;
					else if (is_facing_down)
						s.y = TILE_SIZE / 2;
					door_h.y = horiz_p.y + s.y;
					door_h.x = horiz_p.x +  (s.y / tan(ray_angle));
					door_v = vert_p;
					door_h_dist = distance_between_points(a, door_h);
					door_v_dist = vertical_dist;
					if (door_h_dist < door_v_dist)
					{
						o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
						if (is_facing_up || is_facing_down)
							img = &game->pack.door[0];
						dist_wall = door_h_dist;
						wall_hit = door_h;
						if (is_facing_up)
							offset = (int)((game->doors[door_index].door_prog * img->w) - (o * img->w));
						else if (is_facing_down)
							offset = (int)((game->doors[door_index].door_prog * img->w) - (o * img->w));
					}
					else
					{
						o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
						if (is_facing_left || is_facing_right)
							img = &game->pack.door[1];
						dist_wall = door_v_dist;
						wall_hit = door_v;
						if (is_facing_left)
							offset = img->w - (int)(o * img->w) - 1;
						else if (is_facing_right)
							offset = (int)(o * img->w);
					}
				}
				else
				{
					if (is_facing_left)
						s.x = - TILE_SIZE / 2;
					else if (is_facing_right)
						s.x = TILE_SIZE / 2;
					door_v.x = vert_p.x + s.x;
					door_v.y = vert_p.y + (s.x * tan(ray_angle));
					door_h = horiz_p;
					door_v_dist = distance_between_points(a, door_v);
					door_h_dist = horizontal_dist;
					if (door_h_dist < door_v_dist)
					{
						o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
						if (is_facing_up || is_facing_down)
							img = &game->pack.door[1];
						dist_wall = door_h_dist;
						wall_hit = door_h;
						if (is_facing_up)
							offset = (int)(o * img->w);
						else if (is_facing_down)
							offset = img->w - (int)(o * img->w) - 1;
					}
					else
					{
						o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
						if (is_facing_left || is_facing_right)
							img = &game->pack.door[0];
						dist_wall = door_v_dist;
						wall_hit = door_v;
						if (is_facing_left)
							offset = (int)((game->doors[door_index].door_prog * img->w) - (1.0 - o * img->w));
						else if (is_facing_right)
							offset = (int)((game->doors[door_index].door_prog * img->w) - (1.0 - o * img->w));
					}
				}
			}
			// door.c texture///
			if (is_facing_up && horizontal_dist < vertical_dist)
			{						
				if (img == NULL)
				{
					img = &game->pack.no_img;
					int map_x = (int)floor((wall_hit.x / TILE_SIZE));
					int map_y = (int)floor(wall_hit.y / TILE_SIZE) + 1;
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					offset = (int)(o * img->w);
				}
			}
			else if (is_facing_down && horizontal_dist < vertical_dist)
			{
				if (img == NULL)
				{
					img = &game->pack.so_img;
					int map_x = (int)floor((wall_hit.x / TILE_SIZE));
					int map_y = (int)floor(wall_hit.y / TILE_SIZE) - 1;
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					offset = img->w - (int)(o * img->w) - 1;
				}
			}
			else if (is_facing_right && horizontal_dist > vertical_dist)
			{
				if (img == NULL)
				{
					img = &game->pack.ea_img;
					int map_x = (int)floor((wall_hit.x / TILE_SIZE) - 1);
					int map_y = (int)floor(wall_hit.y / TILE_SIZE);
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					offset = (int)(o * img->w);
				}
			}
			else if (is_facing_left && horizontal_dist > vertical_dist)
			{
				if (img == NULL)
				{
					img = &game->pack.we_img;
					int map_x = (int)floor((wall_hit.x / TILE_SIZE) + 1);
					int map_y = (int)floor(wall_hit.y / TILE_SIZE);
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					offset = img->w - (int)(o * img->w) - 1;
				}
			}
		}
		dist_wall *= cos(game->player.angle - ray_angle);
		if (dist_wall < 0.001)
			dist_wall = 0.001;
		ray_dist[ray] = dist_wall;
		waves = 5 + (cos (20 * (game->player.pos.x + game->player.pos.y)));
		double	line_height = (TILE_SIZE / dist_wall) * (g_global()->project_dist);
		middle = (0.5 + g_global()->look) * HEIGHT + g_global()->j_croutch + waves;
		double draw_begin = middle - 0.5 * line_height;
		draw_begin += waves;
		double	draw_end = draw_begin + line_height;
		// draw_rectangle_rgb_gradient(&game->img, ray * RES, RES, 0, (int)draw_begin, game->txt.c);
		// draw_rectangle_rgb_rgradient(&game->img, ray * RES, RES, (int)draw_end, HEIGHT, game->txt.f);
		if (img != NULL)
			draw_texture(&game->img, img, ray * RES, RES, (int)(draw_begin), (int)(draw_end), offset, dist_wall);
		draw_floor_and_ceil(game, &game->pack.ceil, ray, ray_angle, 0, (int)draw_begin, middle, false);
		draw_floor_and_ceil(game, &game->pack.floor, ray, ray_angle, (int)draw_end + 1, HEIGHT, middle, true);

		ray_angle += adjusted_fov / R_NB;
		ray++;
	}
		//sprite;
	int	sp_c = 0;
	while (sp_c < g_global()->sp_count)
	{
		game->sp[sp_c].d.x = game->sp[sp_c].pos.x - game->player.pos.x;
		game->sp[sp_c].d.y = game->sp[sp_c].pos.y - game->player.pos.y;
		game->sp[sp_c].p = normalize_angle(atan2(game->sp[sp_c].d.y, game->sp[sp_c].d.x));
		game->sp[sp_c].q = normalize_angle(game->sp[sp_c].p - game->player.angle + HALF_FOV);
		game->sp[sp_c].sp_screen.x = game->sp[sp_c].q * (WIDTH / adjusted_fov);
		game->sp[sp_c].sp_screen.y = middle;
		game->sp[sp_c].sp.x = game->sp[sp_c].pos.x * TILE_SIZE;
		game->sp[sp_c].sp.y = game->sp[sp_c].pos.y * TILE_SIZE;
		game->sp[sp_c].dist = distance_between_points(a, game->sp[sp_c].sp);
		sp_c++;
	}
	sort_sprites_by_distance(game->sp);
	static int	shoot_ind = -1;
	static size_t shoot_time = 0;
	sp_c = g_global()->sp_count - 1;
	bool in_crosshair;
	while (sp_c >= 0)
	{
		game->sp[sp_c].proj_sp_height = g_global()->project_dist * TILE_SIZE / game->sp[sp_c].dist;
		int	sprite_height = (int)game->sp[sp_c].proj_sp_height;
		int	sprite_width = sprite_height;
		int	start_y = (int)(game->sp[sp_c].sp_screen.y - (sprite_height / 2) * 0.4);
		int end_y = start_y + sprite_height;
		int	start_x = (int)(game->sp[sp_c].sp_screen.x - sprite_width / 2);
		int	end_x = start_x + sprite_width;
		in_crosshair = (
			HALF_WIDTH >= start_x && HALF_WIDTH < end_x &&
			HALF_HEIGHT >= start_y && HALF_HEIGHT < end_y && game->sp[sp_c].is_died == false
		);
		if (game->player.key.shoot && in_crosshair)
		{
			if (shoot_ind == -1)
			{
				shoot_ind = sp_c;
				shoot_time = get_current_time();
			}
		}
		if (game->player.key.recharge == false)
			shoot_ind = -1;
		sp_c--;
	}
	sp_c = 0;
	while (sp_c < g_global()->sp_count)
	{
		if (game->sp[sp_c].dist > 4)
		{
			if (get_current_time() - shoot_time <= 500 && shoot_ind == sp_c && game->sp[sp_c].is_died == false)
			{
				draw_sprite(game, game->sp[sp_c].sp_screen, game->sp[sp_c].proj_sp_height, &game->pack.pain, ray_dist, game->sp[sp_c].dist);
				if (game->sp[sp_c].was_shot == false)
				{
					game->sp[sp_c].health--;
					game->sp[sp_c].was_shot = true;
					if (game->sp[sp_c].health <= 0 && game->sp[sp_c].was_shot)
					{
						game->sp[sp_c].is_died = true;
						game->sp[sp_c].d_fps = get_current_time();
						game->sp[sp_c].d_frm = 0;
					}
				}
			}
			else if (game->sp[sp_c].is_died == true)
			{
				if (game->sp[sp_c].d_frm <= 6 && get_current_time() - game->sp[sp_c].d_fps <= 10000)
					draw_sprite(game, game->sp[sp_c].sp_screen, game->sp[sp_c].proj_sp_height, &game->pack.death[game->sp[sp_c].d_frm], ray_dist, game->sp[sp_c].dist);
				if (get_current_time() - game->sp[sp_c].d_fps >= 100)
				{
					if (game->sp[sp_c].d_frm < 6)
					{
						game->sp[sp_c].d_frm++;
						game->sp[sp_c].d_fps = get_current_time();
					}
				}
				game->sp[sp_c].was_shot = false;
			}
			else
			{
				draw_sprite(game, game->sp[sp_c].sp_screen, game->sp[sp_c].proj_sp_height, &game->pack.walk[game->sp[sp_c].w_frm], ray_dist, game->sp[sp_c].dist);
				if (get_current_time() - game->sp[sp_c].w_fps >= 300)
				{
					game->sp[sp_c].w_frm++;
					game->sp[sp_c].w_fps = get_current_time();
				}
				if (game->sp[sp_c].w_frm > 3)
				game->sp[sp_c].w_frm = 0;
				game->sp[sp_c].was_shot = false;
			}
		}
		if (game->sp[sp_c].is_died == false && game->sp[sp_c].was_shot == false)
		{
			t_vector move = {game->sp[sp_c].pos.x - cos(game->sp[sp_c].p) * 0.01, game->sp[sp_c].pos.y - sin(game->sp[sp_c].p) * 0.01};
			if (!check_collision(move, game->player, game->map))
			{
				game->sp[sp_c].pos.x = move.x;
				game->sp[sp_c].pos.y = move.y;
			}
			else
			{
				if (!check_collision((t_vector){move.x, game->sp[sp_c].pos.y}, game->player, game->map))
					game->sp[sp_c].pos.x = move.x;
				if (!check_collision((t_vector){game->sp[sp_c].pos.x, move.y}, game->player, game->map))
					game->sp[sp_c].pos.y = move.y;
			}
			float dist_for_die = distance_between_points(game->sp[sp_c].pos, game->player.pos);
			if (dist_for_die < 0.5 && !game->sp[sp_c].is_died)
			{
				game->player.health--;
				paint_image_to_win(&game->img, &game->pack.blood, WIDTH, HEIGHT, (t_vect){0, 0}, -1);
				if (game->player.health <= 0)
					g_global()->game_over = true;
			}
		}
		sp_c++;
	}
	draw_rectangle(&game->img, WIDTH - 301, WIDTH - 99, HEIGHT - 61, HEIGHT - 39, 0x1E1E1E);
	if (game->player.health >= 0)
		draw_rectangle(&game->img, WIDTH - 300, WIDTH - 300 + game->player.health, HEIGHT - 60, HEIGHT - 40, get_health_color((game->player.health / 2), 100));
	t_vect center = {(int)(HALF_WIDTH), (int)(HALF_HEIGHT)};
	if (game->player.key.scope == true)
		paint_image_to_win(&game->img, &game->pack.scope, WIDTH, HEIGHT, (t_vect){0, 0}, -1);
	else
		paint_image_to_win(&game->img, &game->pack.aiming, 30, 30, center, 0xFF0000);
	
	
	t_vector compass = {WIDTH - 121, 95};
	t_vect comp = {WIDTH - 200, 20};
	paint_image_to_win(&game->img, &game->pack.compass, 150, 150, comp, -1);
	draw_rotated_texture(&game->img, &game->pack.needle, compass, game->player.angle, 100, 100);
	draw_minimap(&game->img, game->map, game->player, game);
	t_vect pl = {100 - 7, HEIGHT - 100 - 7};
	paint_image_to_win(&game->img, &game->pack.arrow, 12, 12, pl, 0x00FF00);
	// draw_circle_full(&game->img, p, 4, 0x00FF00);
	t_vect g = {800 - 100, (int)(HEIGHT  - 300 - (g_global()->look * 50) - (waves * 10))};
	if (game->player.key.recharge == true)
	{
		g_global()->updated = true;
		static size_t last_update = 0;
		if (g_global()->gun_frm < 15)
		{
			paint_image_to_win(&game->img, &game->pack.gun_sp[g_global()->gun_frm], 500, 500, g, -1);
			if (get_current_time() - last_update >= FPS)
			{
				g_global()->gun_frm++;
				last_update = get_current_time();
			}
		}
		else
		{
			game->player.key.recharge = false;
			g_global()->gun_frm = 0;
			last_update = 0;
			g_global()->updated = true;
		}
	}
	else if (game->player.key.key_jump == false && game->player.key.scope == false)
	{
		paint_image_to_win(&game->img, &game->pack.gun_sp[0], 500, 500, g, -1);
		g_global()->updated = false;
	}
	paint_image_to_win(&game->img, &game->pack.notice, 350, 100, (t_vect){1, 10}, -1);
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	if (game->player.key.hide_mouse == true)
		mlx_string_put(game->var.mlx, game->var.win, 70, 70, 0xFF0000, "mouse activate");
	if (game->player.key.door_check == true)
		g_global()->updated = true;
	mlx_string_put(game->var.mlx, game->var.win, 70, 50, 0, "Press F1 to display the help menu");
	g_global()->first = false;
	return (0);
}

int	count_sprite(char **map)
{
	int i;
	int	j;
	int	count;

	j = 0;
	count = 0;
	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == sprite)
				count++;
			i++;
		}
		j++;
	}
	return (count);
}

t_sprit init_sprite_data(int x, int y)
{
	t_sprit	sp;

	sp.pos.x = x + 0.5;
	sp.pos.y = y + 0.5;
	sp.w_frm = 0;
	sp.w_fps = get_current_time();
	sp.health = 3;
	sp.d_fps = get_current_time();
	sp.d_frm = 0;
	sp.was_shot = false;
	sp.is_died = false;
	return (sp);
}

t_sprit	*init_sprite(char **map, t_game *game)
{
	int		x;
	int		y;
	t_sprit *sp;
	int		k;

	sp  = malloc(sizeof(t_sprit) * g_global()->sp_count);
	if (!sp)
		return (NULL);
	y = 0;
	k = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == sprite)
			{
				sp[k] = init_sprite_data(x, y);
				k++;
			}
			x++;
		}
		y++;
	}
	return (sp);
}

void	init_game(t_game *game)
{
	game->map = NULL;
	game->var.mlx = NULL;
	game->var.win = NULL;
	game->sp = NULL;
	game->doors = NULL;
	game->img.img_ptr = NULL;
}

void	init_img_loop(t_image *img, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		img[i].img_ptr = NULL;
		i++;
	}
}

void	init_all_images(t_game *game)
{
	game->pack.aiming.img_ptr = NULL;
	game->pack.arrow.img_ptr = NULL;
	game->pack.no_img.img_ptr = NULL;
	game->pack.so_img.img_ptr = NULL;
	game->pack.ea_img.img_ptr = NULL;
	game->pack.we_img.img_ptr = NULL;
	game->pack.floor.img_ptr = NULL;
	game->pack.ceil.img_ptr = NULL;
	game->pack.compass.img_ptr = NULL;
	game->pack.needle.img_ptr = NULL;
	game->pack.scope.img_ptr = NULL;
	game->pack.blood.img_ptr = NULL;
	game->pack.gov.img_ptr = NULL;
	game->pack.notice.img_ptr = NULL;
	game->pack.help.img_ptr = NULL;
	game->pack.pain.img_ptr = NULL;
	game->pack.load.img_ptr = NULL;
	init_img_loop(game->pack.gun_sp, 15);
	init_img_loop(game->pack.door, 2);
	init_img_loop(game->pack.walk, 4);
	init_img_loop(game->pack.death, 7);
	init_img_loop(game->intro.img, 384);
}

void	event_listener(t_game *game)
{
	launch_music("./sounds/disaster_war.mp3", "0.5");
	mlx_hook(game->var.win, DestroyNotify, 0, leave_windows, game);
	mlx_hook(game->var.win, KeyPress, KeyPressMask, ft_key_press, game);
	mlx_hook(game->var.win, KeyRelease, KeyReleaseMask, ft_key_release, game);
	mlx_hook(game->var.win, ButtonPress, ButtonPressMask, ft_mouse_pressed, game);
	mlx_hook(game->var.win, ButtonRelease, ButtonReleaseMask, ft_mouse_release, game);
	mlx_hook(game->var.win, MotionNotify, PointerMotionMask, ft_mouse_move, game);
	mlx_loop_hook(game->var.mlx, game_loop, game);
	mlx_loop(game->var.mlx);
}

int	main(int ac, char **av)
{
	t_game game;

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
	game.sp = init_sprite(game.map, &game);
	event_listener(&game);
	return (0);
}
