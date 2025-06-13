/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:56:54 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/13 15:54:57 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include "type_bonus.h"

// init
void		init_game(t_game *game);
int			init_window(t_game *game);
void		init_img_loop(t_image *img, int nb);
void		init_all_images(t_game *game);
int			init_image_win(t_game *game, t_image *image, int w, int h);
t_g			*g_global(void);

// load_images
void		load_image(t_game *game, t_image *img, char *path);
void		load_pack(t_game *game);
void		load_gun_sp(t_game *game);
void		load_all_images(t_game *game);

// video
void		load_video(t_game *game);
int			video_running(t_game *game);

// PARSE
t_text		ft_parsing(char *path_map, char ***game_map);

// PARSE UTILS
int			ft_ispace(char c);
int			ft_skip_space(char *s);
int			ft_array_lenght(char **arr);

// FREE_UTILS
void		*ft_free(void *param);
void		ft_free_arr(void **arr);
void		ft_free_txt(t_text *txt);
void		init_player(t_game *game);

// KEY_EVENTS
int			ft_key_press(int keysym, t_game *game);
int			ft_key_release(int keysym, t_game *game);

// mouse events
int			ft_mouse_pressed(int button, int x, int y, t_game *game);
int			ft_mouse_release(int button, int x, int y, t_game *game);
int			ft_mouse_move(int x, int y, t_game *game);

// game_loop
int			game_loop(t_game *game);

// ray_cast_images
t_image		*get_image_by_side(t_game *game, t_r *r, t_image *img);

// door
void		check_door_close(t_game *game);
void		close_automatic(t_game *game, t_vector prev_door);
void		open_door(t_game *game, t_vector door_hit, int door_index);
void		close_door(t_game *game, t_vector door_hit, t_vector *prev_door,
				int door_index);

// door_utils
void		init_door(t_door *door, t_cell cell, int pos_x, int pos_y);
t_door		*handle_door(char **map);
int			count_door(char **map);
int			find_door(t_door *doors, int x, int y);
bool		is_hit_door(int x, int y, char **map, t_player player);
bool		is_hit_door_close(int x, int y, char **map, t_player player);
bool		is_hit_door_open(int x, int y, char **map, t_player player);
double		set_hit_and_dist_wall(t_r *r);
t_image		*check_if_hit_door(t_game *game, t_r *r, t_image *img);

// cast_door
t_image		*cast_ray_door(t_game *game, t_r *r);
void		set_door_in_middle(t_r *r, t_d *d);
t_image		*get_door_image_east_and_west(t_game *game, t_r *r, t_d *d,
				t_image *img);
t_image		*get_door_image_north_and_south(t_game *game, t_r *r, t_d *d,
				t_image *img);
void		slide_door_east_and_west(t_r *r, t_d *d);
void		slide_door_north_and_south(t_r *r, t_d *d);

// music
void		stop_music(void);
void		launch_music(char const *file, char *vol);
void		wait_music(void);
size_t		get_current_time(void);
bool		is_music_playing(void);

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

bool		is_hit_door_open(int x, int y, char **map, t_player player);
bool		is_hit_door_close(int x, int y, char **map, t_player player);
bool		is_hit_door(int x, int y, char **map, t_player player);

// drawing
void		draw_sprite(t_game *game, t_image *img, t_r r, t_sprit sp);
void		draw_rotated_texture(t_image *dest, t_image *src, t_squ squ,
				t_vect size);
void		draw_floor_and_ceil(t_game *game, t_image *texture, t_r *r,
				bool fl);

// drawing_utils
int			color_by_distance(int color, double distance, int light_level);
void		draw_circle(t_image *img, t_vector p, int radius, int color);
void		draw_rectangle(t_image *image, t_vect a, t_vect b, int color);
void		draw_circle_full(t_image *img, t_vector p, int radius, int color);
void		draw_rotated_square(t_image *img, t_squ squ, int size, int color);

void		draw_texture(t_image *img, t_image *txt, t_r *r);
void		draw_circle_enemy(t_image *img, t_squ squ, int radius, int color);

// render
void		render_ceil_and_floor(t_game *game, t_r *r);
void		draw_compass(t_game *game);
void		render_gun(t_game *game, t_r *r);
void		render_gun_by_frame(t_game *game, size_t *last_update, t_vect g);
int			game_over(t_game *game);

// render utils
void		put_pixel(t_image *img, int x, int y, int color);
int			set_color_rgb(int r, int g, int b);
int			get_img_color(t_image *img, int x, int y);
void		clear_image(t_image *img, int width, int height, int color);
void		paint_image_to_win(t_image *img, t_image *img1, t_vect s, t_vect p);
void		paint_image_to_win_arrow(t_image *img, t_image *img1, t_vect s,
				t_vect p);
void		paint_image_to_win_aiming(t_image *img, t_image *img1, t_vect s,
				t_vect p);
int			get_health_color(int health, int max_health);
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
void		draw_minimap(t_image *img, t_player player, t_game *game);

// move
void		move_player(t_game *game);
void		look_up_and_down(t_game *game);
void		move_right_and_left(t_game *game, double move_speed);
void		move_up_and_down(t_game *game, double move_speed);

// move_extra
void		jump_player(t_game *game);
void		crouch_player(t_game *game);

// sprites
t_sprit		*init_sprite(char **map);
int			count_sprite(char **map);
void		render_sprites(t_game *game, t_r r, double adjusted_fov);

// sprites_utils
t_sprit		*init_sprite(char **map);
int			count_sprite(char **map);
void		move_sprite(t_game *game, int sp_c);
void		attack_to_player(t_game *game, int sp_c);
void		render_sprite_walk(t_game *game, t_r r, int sp_c);
void		render_sprite_died(t_game *game, t_r r, int sp_c);
void		render_sprite_pain(t_game *game, t_r r, int sp_c);

// free
int			leave_windows(t_game *game);
void		free_all(t_game *game);
void		free_game_var(t_game *game);
void		ft_free_img_packs(t_game *game, t_pack packs);
void		ft_free_img_loop(t_game *game, t_image *img, int nb);
void		ft_free_image(t_game *game, t_image image);

#endif