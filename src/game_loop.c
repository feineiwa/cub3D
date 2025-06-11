/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:34:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/11 08:20:16 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	video_running(t_game *game)
{
	if (g_global()->first == true)
	{
		mlx_put_image_to_window(game->var.mlx, game->var.win,
			game->intro.img[game->intro.frm].img_ptr, 0, 0);
		if (get_current_time() - game->intro.last_time >= 90
			&& game->intro.frm < 383)
		{
			game->intro.frm++;
			game->intro.last_time = get_current_time();
		}
		return (0);
	}
	return (1);
}

int	game_over(t_game *game)
{
	if (g_global()->game_over == true)
	{
		paint_image_to_win(&game->img, &game->pack.gov, (t_vect){WIDTH, HEIGHT},
			(t_vect){0, 0});
		mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr,
			0, 0);
		return (0);
	}
	return (1);
}

int	start_render(t_game *game)
{
	if (!video_running(game))
		return (0);
	if (!game_over(game))
		return (0);
	if (g_global()->help == true)
	{
		paint_image_to_win(&game->img, &game->pack.help, (t_vect){WIDTH,
			HEIGHT}, (t_vect){0, 0});
		mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr,
			0, 0);
		return (0);
	}
	return (1);
}

double	set_dist(bool flag, t_vector a, t_vector b)
{
	if (flag == true)
		return (distance_between_points(a, b));
	return (__DBL_MAX__);
}

t_image	*facing_up(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.no_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE));
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE) + 1;
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = (int)(r->o * img->w);
	}
	return (img);
}

t_image	*facing_down(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.so_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE));
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE) - 1;
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	return (img);
}

t_image	*facing_right(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.ea_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE) - 1);
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE);
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = (int)(r->o * img->w);
	}
	return (img);
}

t_image	*facing_left(t_game *game, t_r *r, t_image *img)
{
	int	map_x;
	int	map_y;

	if (img == NULL)
	{
		img = &game->pack.we_img;
		map_x = (int)floor((r->wall_hit.x / TILE_SIZE) + 1);
		map_y = (int)floor(r->wall_hit.y / TILE_SIZE);
		if (game->map[map_y][map_x] == door_c
			|| game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	return (img);
}

t_image	*get_image_by_side(t_game *game, t_r *r, t_image *img)
{
	if (r->is_facing_up && r->horizontal_dist < r->vertical_dist)
		img = facing_up(game, r, img);
	else if (r->is_facing_down && r->horizontal_dist < r->vertical_dist)
		img = facing_down(game, r, img);
	else if (r->is_facing_right && r->horizontal_dist > r->vertical_dist)
		img = facing_right(game, r, img);
	else if (r->is_facing_left && r->horizontal_dist > r->vertical_dist)
		img = facing_left(game, r, img);
	return (img);
}

void	render_gun_by_frame(t_game *game, size_t *last_update, t_vect g)
{
	if (g_global()->gun_frm < 15)
	{
		paint_image_to_win(&game->img, &game->pack.gun_sp[g_global()->gun_frm],
			(t_vect){500, 500}, g);
		if (get_current_time() - *last_update >= FPS)
		{
			g_global()->gun_frm++;
			*last_update = get_current_time();
		}
	}
	else
	{
		game->player.key.recharge = false;
		g_global()->gun_frm = 0;
		*last_update = 0;
		g_global()->updated = true;
	}
}

void	render_gun(t_game *game, t_r *r)
{
	static size_t	last_update = 0;
	t_vect			g;

	g.x = 800 - 100;
	g.y = (int)(HEIGHT - 300 - (g_global()->look * 50) - (r->waves * 10));
	if (game->player.key.recharge == true)
	{
		g_global()->updated = true;
		render_gun_by_frame(game, &last_update, g);
	}
	else if (game->player.key.key_jump == false
		&& game->player.key.scope == false)
	{
		paint_image_to_win(&game->img, &game->pack.gun_sp[0], (t_vect){500,
			500}, g);
		g_global()->updated = false;
	}
}

void	print_notice(t_game *game)
{
	if (game->player.key.hide_mouse == true)
		mlx_string_put(game->var.mlx, game->var.win, 70, 70, 0xFF0000,
			"mouse activate");
	if (game->player.key.door_check == true)
		g_global()->updated = true;
	mlx_string_put(game->var.mlx, game->var.win, 70, 50, 0,
		"Press F1 to display the help menu");
}

double	set_hit_and_dist_wall(t_r *r)
{
	if (r->horizontal_dist < r->vertical_dist)
	{
		r->wall_hit = r->horiz_p;
		r->dist_wall = fabs(r->horizontal_dist);
		return ((fmod(r->wall_hit.x, TILE_SIZE) / TILE_SIZE));
	}
	r->wall_hit = r->vert_p;
	r->dist_wall = fabs(r->vertical_dist);
	return ((fmod(r->wall_hit.y, TILE_SIZE) / TILE_SIZE));
}

void	extra_for_bonus(t_game *game, t_r *r, double adjusted_fov)
{
	t_squ	squ;
	
	render_sprites(game, *r, adjusted_fov);
	draw_rectangle(&game->img, (t_vect){WIDTH - 301, HEIGHT - 61},
		(t_vect){WIDTH - 99, HEIGHT - 39}, 0x1E1E1E);
	if (game->player.health >= 0)
	{
		draw_rectangle(&game->img, (t_vect){WIDTH - 300, HEIGHT - 60},
			(t_vect){WIDTH - 300 + game->player.health, HEIGHT - 40},
			get_health_color((game->player.health / 2), 100));
	}
	if (game->player.key.scope == true)
		paint_image_to_win(&game->img, &game->pack.scope, (t_vect){WIDTH,
			HEIGHT}, (t_vect){0, 0});
	else
		paint_image_to_win_aiming(&game->img, &game->pack.aiming, (t_vect){30,
			30}, (t_vect){(int)(HALF_WIDTH), (int)(HALF_HEIGHT)});
	paint_image_to_win(&game->img, &game->pack.compass, (t_vect){150, 150},
		(t_vect){WIDTH - 200, 22});
	squ.angle = game->player.angle;
	squ.c = (t_vector){WIDTH - 121, 95};
	draw_rotated_texture(&game->img, &game->pack.needle, squ, (t_vect){100,100});
	draw_minimap(&game->img, game->player, game);
	paint_image_to_win_arrow(&game->img, &game->pack.arrow, (t_vect){12, 12},
		(t_vect){100 - 7, HEIGHT - 100 - 7});
	render_gun(game, r);
	paint_image_to_win(&game->img, &game->pack.notice, (t_vect){350, 100},
		(t_vect){1, 10});
}

void	correct_dist(t_game *game, t_r *r)
{
	r->dist_wall *= cos(game->player.angle - r->ray_angle);
	if (r->dist_wall < 0.001)
		r->dist_wall = 0.001;
	r->ray_dist[r->ray] = r->dist_wall;
}

void	calcul_draw_begin_end_middle_line_heigt(t_game *game, t_r *r)
{
	r->waves = 5 + (cos(20 * (game->player.pos.x + game->player.pos.y)));
	r->line_height = (TILE_SIZE / r->dist_wall) * (g_global()->project_dist);
	r->middle = (0.5 + g_global()->look) * HEIGHT + g_global()->j_croutch
		+ r->waves;
	r->draw_begin = r->middle - 0.5 * r->line_height + r->waves;
	r->draw_end = r->draw_begin + r->line_height;
}

void	init_ray(t_game *game, t_r *r)
{
	r->ray = 0;
	r->horizontal_dist = 0;
	r->vertical_dist = 0;
	r->a.x = game->player.pos.x * TILE_SIZE;
	r->a.y = game->player.pos.y * TILE_SIZE;
	r->adjusted_fov = FOV / g_global()->zoom;
	g_global()->project_dist = (800 / tan(r->adjusted_fov / 2))
	- g_global()->narrow_look;
	r->ray_angle = game->player.angle - (r->adjusted_fov / 2);
}

t_image	*check_if_hit_door(t_game *game, t_r *r, t_image *img)
{
	if (is_hit_door((int)floor(r->wall_hit.x), (int)floor(r->wall_hit.y),
			game->map, game->player))
	{
		img = cast_ray_door(game, r);
		img = get_image_by_side(game, r, img);
	}
	return (img);
}

void	ray_cast_loop(t_game *game, t_r *r)
{
	t_image	*img;

	while (r->ray < R_NB)
	{
		r->ray_angle = normalize_angle(r->ray_angle);
		compute_ray_orientation(r->ray_angle, r);
		r->horiz_p = ray_cast_horizontal(game, r);
		r->vert_p = ray_cast_vertical(game, r);
		r->horizontal_dist = set_dist(r->found_horizontal, r->a, r->horiz_p);
		r->vertical_dist = set_dist(r->found_vertical, r->a, r->vert_p);
		r->o = set_hit_and_dist_wall(r);
		img = NULL;
		img = get_image_by_side(game, r, img);
		img = check_if_hit_door(game, r, img);
		correct_dist(game, r);
		calcul_draw_begin_end_middle_line_heigt(game, r);
		draw_texture(&game->img, img, r);
		draw_floor_and_ceil(game, &game->pack.ceil, r, false);
		draw_floor_and_ceil(game, &game->pack.floor, r, true);
		// draw_rectangle_rgb_gradient(&game->img, r->ray * RES, RES, 0,
		// (int)r->draw_begin, game->txt.c);
		// draw_rectangle_rgb_rgradient(&game->img, r->ray * RES, RES,
		// (int)r->draw_end, HEIGHT, game->txt.f);
		r->ray_angle += r->adjusted_fov / R_NB;
		r->ray++;
	}
}

int	game_loop(t_game *game)
{
	t_r	r;

	// if (g_global()->updated == false && g_global()->first == false)
	// 	return (1);
	if (!start_render(game))
		return (0);
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT, 0);
	check_door_close(game);
	move_player(game);
	init_ray(game, &r);
	ray_cast_loop(game, &r);
	extra_for_bonus(game, &r, r.adjusted_fov);
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	print_notice(game);
	g_global()->first = false;
	return (0);
}
