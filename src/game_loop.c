/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:34:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 00:37:37 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	print_notice(t_game *game)
{
	if (game->player.key.hide_mouse == true)
		mlx_string_put(game->var.mlx, game->var.win, 70, 70, 0xFF0000,
			"mouse activate");
	if (game->player.key.door_check == true)
		g_global()->updated = true;
	mlx_string_put(game->var.mlx, game->var.win, 70, 50, 0,
		"Press F1 to display the help menu");
}

static void	extra_for_bonus(t_game *game, t_r *r, double adjusted_fov)
{
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
	draw_compass(game);
	draw_minimap(&game->img, game->player, game);
	paint_image_to_win_arrow(&game->img, &game->pack.arrow, (t_vect){12, 12},
		(t_vect){100 - 7, HEIGHT - 100 - 7});
	render_gun(game, r);
	paint_image_to_win(&game->img, &game->pack.notice, (t_vect){350, 100},
		(t_vect){1, 10});
}

static void	ray_cast_loop(t_game *game, t_r *r)
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
		if (game->bonus)
			img = check_if_hit_door(game, r, img);
		correct_dist(game, r);
		calcul_draw_begin_end_middle_line_heigt(game, r);
		draw_texture(&game->img, img, r);
		render_ceil_and_floor(game, r);
		r->ray_angle += r->adjusted_fov / R_NB;
		r->ray++;
	}
}

static int	start_render(t_game *game)
{
	if (!video_running(game))
		return (0);
	else
		stop_music();
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

int	game_loop(t_game *game)
{
	t_r	r;

	if (game->bonus && !start_render(game))
		return (0);
	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT, 0);
	if (game->bonus)
		check_door_close(game);
	move_player(game);
	init_ray(game, &r);
	ray_cast_loop(game, &r);
	if (game->bonus)
		extra_for_bonus(game, &r, r.adjusted_fov);
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	if (game->bonus)
		print_notice(game);
	g_global()->first = false;
	return (0);
}
