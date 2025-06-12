/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 08:19:34 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/12 23:56:22 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	compute_ray_orientation(double angle, t_r *r)
{
	r->is_facing_up = false;
	r->is_facing_down = false;
	r->is_facing_right = false;
	r->is_facing_left = false;
	if (angle > 0 && angle < PI)
		r->is_facing_down = true;
	else
		r->is_facing_up = true;
	if (angle < 1.57079632679 || angle > 4.71238898038)
		r->is_facing_right = true;
	else
		r->is_facing_left = true;
	r->found_horizontal = false;
	r->found_vertical = false;
}

static t_vector	cast_ray_horizontal(t_game *game, t_r *r, t_vector next_horiz)
{
	t_vector	horiz_p;

	while (next_horiz.x < (game->player.w * TILE_SIZE) && next_horiz.x >= 0
		&& next_horiz.y < (game->player.h * TILE_SIZE) && next_horiz.y >= 0)
	{
		if (is_wall_door((int)floor(next_horiz.x), (int)floor(next_horiz.y),
				game->map, game->player))
		{
			r->found_horizontal = true;
			horiz_p = next_horiz;
			break ;
		}
		else
		{
			next_horiz.x += r->s.x;
			next_horiz.y += r->s.y;
		}
	}
	return (horiz_p);
}

t_vector	ray_cast_horizontal(t_game *game, t_r *r)
{
	t_vector	first_int;
	t_vector	next_horiz;

	first_int = (t_vector){0.f, 0.f};
	if (r->is_facing_up == true)
		first_int.y = (floor(game->player.pos.y) - 1e-6) * TILE_SIZE;
	else if (r->is_facing_down == true)
		first_int.y = floor(game->player.pos.y) * TILE_SIZE + TILE_SIZE;
	first_int.x = r->a.x + (first_int.y - r->a.y) / tan(r->ray_angle);
	next_horiz = (t_vector){first_int.x, first_int.y};
	if (r->is_facing_up == true)
		r->s.y = -TILE_SIZE;
	if (r->is_facing_down == true)
		r->s.y = TILE_SIZE;
	r->s.x = r->s.y / tan(r->ray_angle);
	return (cast_ray_horizontal(game, r, next_horiz));
}

static t_vector	cast_ray_vertical(t_game *game, t_r *r, t_vector next_vert)
{
	t_vector	vert_p;

	while (next_vert.x < (game->player.w * TILE_SIZE) && next_vert.x >= 0
		&& next_vert.y < (game->player.h * TILE_SIZE) && next_vert.y >= 0)
	{
		if (is_wall_door((int)floor(next_vert.x), (int)floor(next_vert.y),
				game->map, game->player))
		{
			r->found_vertical = true;
			vert_p = next_vert;
			break ;
		}
		else
		{
			next_vert.x += r->s.x;
			next_vert.y += r->s.y;
		}
	}
	return (vert_p);
}

t_vector	ray_cast_vertical(t_game *game, t_r *r)
{
	t_vector	first_int;
	t_vector	next_vert;

	first_int = (t_vector){0.f, 0.f};
	if (r->is_facing_right == true)
		first_int.x = floor(game->player.pos.x) * TILE_SIZE + TILE_SIZE;
	else if (r->is_facing_left == true)
		first_int.x = (floor(game->player.pos.x) - 1e-6) * TILE_SIZE;
	first_int.y = r->a.y + ((first_int.x - r->a.x) * tan(r->ray_angle));
	next_vert = (t_vector){first_int.x, first_int.y};
	if (r->is_facing_right == true)
		r->s.x = TILE_SIZE;
	else if (r->is_facing_left == true)
		r->s.x = -TILE_SIZE;
	r->s.y = r->s.x * tan(r->ray_angle);
	return (cast_ray_vertical(game, r, next_vert));
}
