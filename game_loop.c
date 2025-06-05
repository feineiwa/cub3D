/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:34:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/05 17:47:41 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "inc/cub3d.h"

int	start_render(t_game *game)
{
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
	return (1);
}
double	set_dist(bool flag, t_vector a, t_vector b)
{
	if (flag == true)
		return (distance_between_points(a, b));
	return (__DBL_MAX__);
}

t_image	*get_image_by_side(t_game *game, t_r *r, t_image *img)
{
	if (r->is_facing_up && r->horizontal_dist < r->vertical_dist)
	{
		if (img == NULL)
			img = &game->pack.no_img;
		int map_x = (int)floor((r->wall_hit.x / TILE_SIZE));
		int map_y = (int)floor(r->wall_hit.y / TILE_SIZE) + 1;
		if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = (int)(r->o * img->w);
	}
	else if (r->is_facing_down && r->horizontal_dist < r->vertical_dist)
	{
		if (img == NULL)
			img = &game->pack.so_img;
		int map_x = (int)floor((r->wall_hit.x / TILE_SIZE));
		int map_y = (int)floor(r->wall_hit.y / TILE_SIZE) - 1;
		if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	else if (r->is_facing_right && r->horizontal_dist > r->vertical_dist)
	{
		if (img == NULL)
			img = &game->pack.ea_img;
		int map_x = (int)floor((r->wall_hit.x / TILE_SIZE) - 1);
		int map_y = (int)floor(r->wall_hit.y / TILE_SIZE);
		if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = (int)(r->o * img->w);
	}
	else if (r->is_facing_left && r->horizontal_dist > r->vertical_dist)
	{
		if (img == NULL)
			img = &game->pack.we_img;
		int map_x = (int)floor((r->wall_hit.x / TILE_SIZE) + 1);
		int map_y = (int)floor(r->wall_hit.y / TILE_SIZE);
		if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
			img = &game->pack.door[1];
		r->offset = img->w - (int)(r->o * img->w) - 1;
	}
	return (img);
}

int	game_loop(t_game *game)
{
	t_r r;
	bool	found_h_o_door = false;
	bool	found_v_o_door = false;
	t_vector door_v;
	t_vector door_h;
	double	adjusted_fov;
	t_image *img;

	
	r.ray = 0;
	r.horizontal_dist = 0;
	r.vertical_dist = 0;
	
	// if (g_global()->updated == false && g_global()->first == false)
	// 	return (1);
	if (!start_render(game))
		return (0);
	mlx_clear_window(game->var.mlx, game->var.win);
	check_door_close(game);
	move_player(game);
	r.a.x = game->player.pos.x * TILE_SIZE;
	r.a.y = game->player.pos.y * TILE_SIZE;
	adjusted_fov = FOV / g_global()->zoom;
	g_global()->project_dist = (HALF_WIDTH / tan(adjusted_fov / 2)) - g_global()->narrow_look;
	r.ray_angle = game->player.angle - (adjusted_fov / 2);
	while (r.ray < R_NB)
	{
		r.ray_angle = normalize_angle(r.ray_angle);
		compute_ray_orientation(r.ray_angle, &r);
		r.found_horizontal = false;
		r.found_vertical = false;
		found_h_o_door = false;
		found_v_o_door = false;
		r.horiz_p = ray_cast_horizontal(game , r.ray_angle, r.is_facing_up, r.is_facing_down, &found_h_o_door, &r.found_horizontal);
		r.vert_p = ray_cast_vertical(game , r.ray_angle, r.is_facing_left, r.is_facing_right, &found_v_o_door, &r.found_vertical);
		// // distance calculation
		r.horizontal_dist =  set_dist(r.found_horizontal, r.a, r.horiz_p);
		r.vertical_dist = set_dist(r.found_vertical, r.a, r.vert_p);
		if (r.horizontal_dist < r.vertical_dist)
		{
			r.wall_hit = r.horiz_p;
			r.dist_wall = fabs(r.horizontal_dist);
			r.o = ((fmod(r.wall_hit.x, TILE_SIZE) / TILE_SIZE));
		}
		else
		{
			r.wall_hit = r.vert_p;
			r.dist_wall = fabs(r.vertical_dist);
			r.o = ((fmod(r.wall_hit.y, TILE_SIZE) / TILE_SIZE));
		}
		img = NULL;
		img = get_image_by_side(game, &r, img);
		if (is_hit_door((int)floor(r.wall_hit.x), (int)floor(r.wall_hit.y), game->map, game->player))
		{
			img = NULL;
			double door_v_dist;
			double	door_h_dist;
			int	door_index = find_door(game->doors, (int)floor(r.wall_hit.x / TILE_SIZE), (int)floor(r.wall_hit.y / TILE_SIZE));
			if (r.horizontal_dist < r.vertical_dist)
			{
				if (r.is_facing_up)
					r.s.y = - TILE_SIZE / 2;
				else if (r.is_facing_down)
					r.s.y = TILE_SIZE / 2;
				door_h.y = r.horiz_p.y + r.s.y;
				door_h.x = r.horiz_p.x +  (r.s.y / tan(r.ray_angle));
				door_v = r.vert_p;
				door_h_dist = distance_between_points(r.a, door_h);
				door_v_dist = r.vertical_dist;
				if (door_h_dist < door_v_dist)
					r.o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
				else
					r.o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
			}
			else
			{
				if (r.is_facing_left)
					r.s.x = - TILE_SIZE / 2;
				else if (r.is_facing_right)
					r.s.x = TILE_SIZE / 2;
				door_v.x = r.vert_p.x + r.s.x;
				door_v.y = r.vert_p.y + (r.s.x * tan(r.ray_angle));
				door_h = r.horiz_p;
				door_v_dist = distance_between_points(r.a, door_v);
				door_h_dist = r.horizontal_dist;
				if (door_h_dist < door_v_dist)
					r.o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
				else
					r.o = 1.0 - ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
			}
			if (r.o <= game->doors[door_index].door_prog)
			{
				t_vector	player;

				player = game->player.pos;
				if (r.horizontal_dist < r.vertical_dist)
				{
					r.found_horizontal = false;
					found_h_o_door = false;
					game->player.pos.x = (r.horiz_p.x / TILE_SIZE);
					game->player.pos.y = (r.horiz_p.y / TILE_SIZE);
					r.horiz_p = ray_cast_horizontal(game, r.ray_angle, r.is_facing_up, r.is_facing_down, &found_h_o_door, &r.found_horizontal);
					r.horizontal_dist = set_dist(r.found_horizontal, r.a, r.horiz_p);
				}
				else
				{
					r.found_vertical = false;
					found_v_o_door = false;
					game->player.pos.x = (r.vert_p.x / TILE_SIZE);
					game->player.pos.y = (r.vert_p.y / TILE_SIZE);
					r.vert_p = ray_cast_vertical(game, r.ray_angle, r.is_facing_left, r.is_facing_right, &found_v_o_door, &r.found_vertical);
					r.vertical_dist = set_dist(r.found_horizontal, r.a, r.vert_p);
					
				}
				game->player.pos = player;
				if (r.horizontal_dist < r.vertical_dist)
				{
					r.wall_hit = r.horiz_p;
					r.dist_wall = fabs(r.horizontal_dist);
					r.o = ((fmod(r.wall_hit.x, TILE_SIZE) / TILE_SIZE));
				}
				else
				{
					r.wall_hit = r.vert_p;
					r.dist_wall = fabs(r.vertical_dist);
					r.o = ((fmod(r.wall_hit.y, TILE_SIZE) / TILE_SIZE));
				}
				if (is_hit_door((int)floor(r.wall_hit.x), (int)floor(r.wall_hit.y), game->map, game->player))
				{
					if (r.horizontal_dist < r.vertical_dist)
					{
						if (r.is_facing_up)
							r.s.y = - TILE_SIZE / 2;
						else if (r.is_facing_down)
							r.s.y = TILE_SIZE / 2;
						door_h.y = r.horiz_p.y + r.s.y;
						door_h.x = r.horiz_p.x +  (r.s.y / tan(r.ray_angle));
						door_v = r.vert_p;
						door_h_dist = distance_between_points(r.a, door_h);
						door_v_dist = r.vertical_dist;
						if (door_h_dist < door_v_dist)
						{
							r.o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
							if (r.is_facing_up || r.is_facing_down)
								img = &game->pack.door[0];
							r.dist_wall = door_h_dist;
							r.wall_hit = door_h;
							if (r.is_facing_up)
								r.offset = (int)(1.0 - r.o * img->w);
							else if (r.is_facing_down)
								r.offset = (int)(1.0 - r.o * img->w);
						}
						else
						{
							r.o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
							if (r.is_facing_left || r.is_facing_right)
								img = &game->pack.door[1];
							r.dist_wall = door_v_dist;
							r.wall_hit = door_v;
							if (r.is_facing_left)
								r.offset = (int)(1.0 - r.o * img->w);
							else if (r.is_facing_right)
								r.offset = (int)(r.o * img->w);
						}
					}
					else
					{
						if (r.is_facing_left)
							r.s.x = - TILE_SIZE / 2;
						else if (r.is_facing_right)
							r.s.x = TILE_SIZE / 2;
						door_v.x = r.vert_p.x + r.s.x;
						door_v.y = r.vert_p.y + (r.s.x * tan(r.ray_angle));
						door_h = r.horiz_p;
						door_v_dist = distance_between_points(r.a, door_v);
						door_h_dist = r.horizontal_dist;
						if (door_h_dist < door_v_dist)
						{
							r.o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
							if (r.is_facing_up || r.is_facing_down)
								img = &game->pack.door[1];
							r.dist_wall = door_h_dist;
							r.wall_hit = door_h;
							if (r.is_facing_up)
								r.offset = (int)(r.o * img->w);
							else if (r.is_facing_down)
								r.offset = (int)(1.0 - r.o * img->w);
						}
						else
						{
							r.o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
							if (r.is_facing_left || r.is_facing_right)
								img = &game->pack.door[0];
							r.dist_wall = door_v_dist;
							r.wall_hit = door_v;
							if (r.is_facing_left)
								r.offset = (int)(r.o * img->w);
							else if (r.is_facing_right)
								r.offset = (int)(r.o * img->w);
						}
					}
				}
			}
			else
			{
				if (r.horizontal_dist < r.vertical_dist)
				{
					if (r.is_facing_up)
						r.s.y = - TILE_SIZE / 2;
					else if (r.is_facing_down)
						r.s.y = TILE_SIZE / 2;
					door_h.y = r.horiz_p.y + r.s.y;
					door_h.x = r.horiz_p.x +  (r.s.y / tan(r.ray_angle));
					door_v = r.vert_p;
					door_h_dist = distance_between_points(r.a, door_h);
					door_v_dist = r.vertical_dist;
					if (door_h_dist < door_v_dist)
					{
						r.o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
						if (r.is_facing_up || r.is_facing_down)
							img = &game->pack.door[0];
						r.dist_wall = door_h_dist;
						r.wall_hit = door_h;
						if (r.is_facing_up)
							r.offset = (int)((game->doors[door_index].door_prog * img->w) - (r.o * img->w));
						else if (r.is_facing_down)
							r.offset = (int)((game->doors[door_index].door_prog * img->w) - (r.o * img->w));
					}
					else
					{
						r.o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
						if (r.is_facing_left || r.is_facing_right)
							img = &game->pack.door[1];
						r.dist_wall = door_v_dist;
						r.wall_hit = door_v;
						if (r.is_facing_left)
							r.offset = img->w - (int)(r.o * img->w) - 1;
						else if (r.is_facing_right)
							r.offset = (int)(r.o * img->w);
					}
				}
				else
				{
					if (r.is_facing_left)
						r.s.x = - TILE_SIZE / 2;
					else if (r.is_facing_right)
						r.s.x = TILE_SIZE / 2;
					door_v.x = r.vert_p.x + r.s.x;
					door_v.y = r.vert_p.y + (r.s.x * tan(r.ray_angle));
					door_h = r.horiz_p;
					door_v_dist = distance_between_points(r.a, door_v);
					door_h_dist = r.horizontal_dist;
					if (door_h_dist < door_v_dist)
					{
						r.o = ((fmod(door_h.x, TILE_SIZE) / TILE_SIZE));
						if (r.is_facing_up || r.is_facing_down)
							img = &game->pack.door[1];
						r.dist_wall = door_h_dist;
						r.wall_hit = door_h;
						if (r.is_facing_up)
							r.offset = (int)(r.o * img->w);
						else if (r.is_facing_down)
							r.offset = img->w - (int)(r.o * img->w) - 1;
					}
					else
					{
						r.o = ((fmod(door_v.y, TILE_SIZE) / TILE_SIZE));
						if (r.is_facing_left || r.is_facing_right)
							img = &game->pack.door[0];
						r.dist_wall = door_v_dist;
						r.wall_hit = door_v;
						if (r.is_facing_left)
							r.offset = (int)((game->doors[door_index].door_prog * img->w) - (1.0 - r.o * img->w));
						else if (r.is_facing_right)
							r.offset = (int)((game->doors[door_index].door_prog * img->w) - (1.0 - r.o * img->w));
					}
				}
			}
			// door.c texture///
			if (r.is_facing_up && r.horizontal_dist < r.vertical_dist)
			{						
				if (img == NULL)
				{
					img = &game->pack.no_img;
					int map_x = (int)floor((r.wall_hit.x / TILE_SIZE));
					int map_y = (int)floor(r.wall_hit.y / TILE_SIZE) + 1;
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					r.offset = (int)(r.o * img->w);
				}
			}
			else if (r.is_facing_down && r.horizontal_dist < r.vertical_dist)
			{
				if (img == NULL)
				{
					img = &game->pack.so_img;
					int map_x = (int)floor((r.wall_hit.x / TILE_SIZE));
					int map_y = (int)floor(r.wall_hit.y / TILE_SIZE) - 1;
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					r.offset = img->w - (int)(r.o * img->w) - 1;
				}
			}
			else if (r.is_facing_right && r.horizontal_dist > r.vertical_dist)
			{
				if (img == NULL)
				{
					img = &game->pack.ea_img;
					int map_x = (int)floor((r.wall_hit.x / TILE_SIZE) - 1);
					int map_y = (int)floor(r.wall_hit.y / TILE_SIZE);
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					r.offset = (int)(r.o * img->w);
				}
			}
			else if (r.is_facing_left && r.horizontal_dist > r.vertical_dist)
			{
				if (img == NULL)
				{
					img = &game->pack.we_img;
					int map_x = (int)floor((r.wall_hit.x / TILE_SIZE) + 1);
					int map_y = (int)floor(r.wall_hit.y / TILE_SIZE);
					if (game->map[map_y][map_x] == door_c || game->map[map_y][map_x] == door_o)
						img = &game->pack.door[1];
					r.offset = img->w - (int)(r.o * img->w) - 1;
				}
			}
		}
		r.dist_wall *= cos(game->player.angle - r.ray_angle);
		if (r.dist_wall < 0.001)
			r.dist_wall = 0.001;
		r.ray_dist[r.ray] = r.dist_wall;
		r.waves = 5 + (cos (20 * (game->player.pos.x + game->player.pos.y)));
		double	line_height = (TILE_SIZE / r.dist_wall) * (g_global()->project_dist);
		r.middle = (0.5 + g_global()->look) * HEIGHT + g_global()->j_croutch + r.waves;
		double draw_begin = r.middle - 0.5 * line_height;
		draw_begin += r.waves;
		double	draw_end = draw_begin + line_height;
		// draw_rectangle_rgb_gradient(&game->img, r.ray * RES, RES, 0, (int)draw_begin, game->txt.c);
		// draw_rectangle_rgb_rgradient(&game->img, r.ray * RES, RES, (int)draw_end, HEIGHT, game->txt.f);
		if (img != NULL)
			draw_texture(&game->img, img, r.ray * RES, RES, (int)(draw_begin), (int)(draw_end), r.offset, r.dist_wall);
		draw_floor_and_ceil(game, &game->pack.ceil, r.ray, r.ray_angle, 0, (int)draw_begin, r.middle, false);
		draw_floor_and_ceil(game, &game->pack.floor, r.ray, r.ray_angle, (int)draw_end + 1, HEIGHT, r.middle, true);

		r.ray_angle += adjusted_fov / R_NB;
		r.ray++;
	}
	render_sprites(game, r, adjusted_fov);
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
	t_vect g = {800 - 100, (int)(HEIGHT  - 300 - (g_global()->look * 50) - (r.waves * 10))};
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
