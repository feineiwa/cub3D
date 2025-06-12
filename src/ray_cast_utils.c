/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 23:52:43 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 00:26:54 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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

void	correct_dist(t_game *game, t_r *r)
{
	r->dist_wall *= cos(game->player.angle - r->ray_angle);
	if (r->dist_wall < 0.001)
		r->dist_wall = 0.001;
	r->ray_dist[r->ray] = r->dist_wall;
}

void	calcul_draw_begin_end_middle_line_heigt(t_game *game, t_r *r)
{
	if (game->bonus)
		r->waves = 5 + (cos(20 * (game->player.pos.x + game->player.pos.y)));
	else
		r->waves = 0;
	r->line_height = (TILE_SIZE / r->dist_wall) * (g_global()->project_dist);
	r->middle = (0.5 + g_global()->look) * HEIGHT + g_global()->j_croutch
		+ r->waves;
	r->draw_begin = r->middle - 0.5 * r->line_height + r->waves;
	r->draw_end = r->draw_begin + r->line_height;
}

double	set_dist(bool flag, t_vector a, t_vector b)
{
	if (flag == true)
		return (distance_between_points(a, b));
	return (__DBL_MAX__);
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
