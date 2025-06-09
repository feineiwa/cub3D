/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:30:31 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/09 17:30:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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

int	count_sprite(char **map)
{
	int	i;
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

void	update_sprites(t_game *game, t_r r, double adjusted_fov)
{
	int	sp_c;

	sp_c = 0;
	while (sp_c < g_global()->sp_count)
	{
		game->sp[sp_c].d.x = game->sp[sp_c].pos.x - game->player.pos.x;
		game->sp[sp_c].d.y = game->sp[sp_c].pos.y - game->player.pos.y;
		game->sp[sp_c].p = normalize_angle(atan2(game->sp[sp_c].d.y,
					game->sp[sp_c].d.x));
		game->sp[sp_c].q = normalize_angle(game->sp[sp_c].p - game->player.angle
				+ HALF_FOV);
		game->sp[sp_c].sp_screen.x = game->sp[sp_c].q * (WIDTH / adjusted_fov);
		game->sp[sp_c].sp_screen.y = r.middle;
		game->sp[sp_c].sp.x = game->sp[sp_c].pos.x * TILE_SIZE;
		game->sp[sp_c].sp.y = game->sp[sp_c].pos.y * TILE_SIZE;
		game->sp[sp_c].dist = distance_between_points(r.a, game->sp[sp_c].sp);
		sp_c++;
	}
}

void	check_sprites_fired(t_game *game, int *shoot_ind, size_t *shoot_time)
{
	int		sp_c;
	bool	in_crosshair;
	int		sprite_height;
	t_vect	start;
	t_vect	end;
	
	sp_c = g_global()->sp_count - 1;
	while (sp_c >= 0)
	{
		game->sp[sp_c].proj_sp_height = g_global()->project_dist * TILE_SIZE
			/ game->sp[sp_c].dist;
		sprite_height = (int)game->sp[sp_c].proj_sp_height;
		start.y = (int)(game->sp[sp_c].sp_screen.y - (sprite_height / 2)
				* 0.4);
		end.y = start.y + sprite_height;
		start.x = (int)(game->sp[sp_c].sp_screen.x - sprite_height / 2);
		end.x = start.x + sprite_height;
		in_crosshair = (HALF_WIDTH >= start.x && HALF_WIDTH < end.x
				&& HALF_HEIGHT >= start.y && HALF_HEIGHT < end.y
				&& game->sp[sp_c].is_died == false);
		if (game->player.key.shoot && in_crosshair)
		{
			if (*shoot_ind == -1)
			{
				*shoot_ind = sp_c;
				*shoot_time = get_current_time();
			}
		}
		if (game->player.key.recharge == false)
			*shoot_ind = -1;
		sp_c--;
	}
}

void	render_sprites(t_game *game, t_r r, double adjusted_fov)
{
	int				sp_c;
	static int		shoot_ind = -1;
	static size_t	shoot_time = 0;

	update_sprites(game, r ,adjusted_fov);
	sort_sprites_by_distance(game->sp);
	check_sprites_fired(game, &shoot_ind, &shoot_time);
	sp_c = 0;
	while (sp_c < g_global()->sp_count)
	{
		if (game->sp[sp_c].dist > 4)
		{
			if (get_current_time() - shoot_time <= 500 && shoot_ind == sp_c
				&& game->sp[sp_c].is_died == false)
			{
				draw_sprite(game, game->sp[sp_c].sp_screen,
					game->sp[sp_c].proj_sp_height, &game->pack.pain, r.ray_dist,
					game->sp[sp_c].dist);
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
				if (game->sp[sp_c].d_frm <= 6 && get_current_time()
					- game->sp[sp_c].d_fps <= 10000)
					draw_sprite(game, game->sp[sp_c].sp_screen,
						game->sp[sp_c].proj_sp_height,
						&game->pack.death[game->sp[sp_c].d_frm], r.ray_dist,
						game->sp[sp_c].dist);
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
				draw_sprite(game, game->sp[sp_c].sp_screen,
					game->sp[sp_c].proj_sp_height,
					&game->pack.walk[game->sp[sp_c].w_frm], r.ray_dist,
					game->sp[sp_c].dist);
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
			t_vector move = {game->sp[sp_c].pos.x - cos(game->sp[sp_c].p)
				* 0.02, game->sp[sp_c].pos.y - sin(game->sp[sp_c].p) * 0.02};
			if (!check_collision(move, game->player, game->map))
			{
				game->sp[sp_c].pos.x = move.x;
				game->sp[sp_c].pos.y = move.y;
			}
			else
			{
				if (!check_collision((t_vector){move.x, game->sp[sp_c].pos.y},
						game->player, game->map))
					game->sp[sp_c].pos.x = move.x;
				if (!check_collision((t_vector){game->sp[sp_c].pos.x, move.y},
						game->player, game->map))
					game->sp[sp_c].pos.y = move.y;
			}
			float dist_for_die = distance_between_points(game->sp[sp_c].pos,
					game->player.pos);
			if (dist_for_die < 0.5 && !game->sp[sp_c].is_died)
			{
				game->player.health--;
				paint_image_to_win(&game->img, &game->pack.blood,
					(t_vect){WIDTH, HEIGHT}, (t_vect){0, 0});
				if (game->player.health <= 0)
					g_global()->game_over = true;
			}
		}
		sp_c++;
	}
}