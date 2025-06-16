/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:56:54 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/16 11:53:11 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "parse.h"

// init
void		init_game(t_game *game);
int			init_window(t_game *game);
void		init_img_loop(t_image *img, int nb);
void		init_all_images(t_game *game);
int			init_image_win(t_game *game, t_image *image, int w, int h);
t_g			*g_global(void);

// load_images
void		load_image(t_game *game, t_image *img, char *path);
void		load_all_images(t_game *game);

// FREE_UTILS
void		*ft_free(void *param);
void		ft_free_arr(void **arr);
void		ft_free_txt(t_text *txt);
void		init_player(t_game *game);

// KEY_EVENTS
int			ft_key_press(int keysym, t_game *game);
int			ft_key_release(int keysym, t_game *game);

// game_loop
int			game_loop(t_game *game);

// ray_cast_images
t_image		*get_image_by_side(t_game *game, t_r *r, t_image *img);

//
bool		is_wall_door(int x, int y, char **map, t_player player);
double		normalize_angle(double angle);
double		distance_between_points(t_vector a, t_vector b);

// ray_cast
void		compute_ray_orientation(double angle, t_r *r);
t_vector	ray_cast_horizontal(t_game *game, t_r *r);
t_vector	ray_cast_vertical(t_game *game, t_r *r);

// ray_cast_utils
void		init_ray(t_game *game, t_r *r);
void		calcul_draw_begin_end_middle_line_heigt(t_game *game, t_r *r);
void		correct_dist(t_game *game, t_r *r);
double		set_dist(bool flag, t_vector a, t_vector b);
double		set_hit_and_dist_wall(t_r *r);

// drawing_utils
void		draw_rectangle(t_image *image, t_vect a, t_vect b, int color);

void		draw_texture(t_image *img, t_image *txt, t_r *r);

// render
void		render_ceil_and_floor(t_game *game, t_r *r);

// render utils
void		put_pixel(t_image *img, int x, int y, int color);
int			set_color_rgb(int r, int g, int b);
int			get_img_color(t_image *img, int x, int y);
void		clear_image(t_image *img, int width, int height, int color);
// collision
bool		is_exceeded(t_vector move, t_player player);
bool		is_hit_wall(int tile_x, int tile_y, char **map);
bool		is_collision(t_vector move, char **map);
bool		check_collision(t_vector move, t_player player, char **map);
bool		is_wall_door(int x, int y, char **map, t_player player);

// calcul
double		distance_between_points(t_vector a, t_vector b);
double		normalize_angle(double angle);

// minimap.c

// move
void		move_player(t_game *game);
void		move_right_and_left(t_game *game, double move_speed);
void		move_up_and_down(t_game *game, double move_speed);

// free
int			leave_windows(t_game *game);
void		free_all(t_game *game);
void		free_game_var(t_game *game);
void		ft_free_img_packs(t_game *game, t_pack packs);
void		ft_free_img_loop(t_game *game, t_image *img, int nb);
void		ft_free_image(t_game *game, t_image image);

#endif