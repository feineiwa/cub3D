/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:34:02 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:39:30 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
		correct_dist(game, r);
		calcul_draw_begin_end_middle_line_heigt(game, r);
		draw_texture(&game->img, img, r);
		render_ceil_and_floor(game, r);
		r->ray_angle += r->adjusted_fov / R_NB;
		r->ray++;
	}
}

int	game_loop(t_game *game)
{
	t_r	r;

	mlx_clear_window(game->var.mlx, game->var.win);
	clear_image(&game->img, WIDTH, HEIGHT, 0);
	move_player(game);
	init_ray(game, &r);
	ray_cast_loop(game, &r);
	mlx_put_image_to_window(game->var.mlx, game->var.win, game->img.img_ptr, 0,
		0);
	return (0);
}
