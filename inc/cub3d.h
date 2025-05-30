/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:56:54 by frahenin          #+#    #+#             */
/*   Updated: 2025/05/30 11:19:17 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../gnl/get_next_line.h"
# include "../libft/libft.h"
# include "../mlx/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef BONUS
#  define BONUS 0
# endif

# define PI 3.14159265359
# define MOVE_SPEED 0.06
# define ROT_SPEED 0.05
# define MOUSE_SPEED 0.0005;
# define RES 1
# define TILE_SIZE 32
# define WIDTH 1600
# define HEIGHT 960
# define HALF_HEIGHT 480
# define HALF_WIDTH 800
# define FOV 1.0471975512
# define HALF_FOV 0.523598776
# define R_NB (WIDTH / RES)
# define MINI_SIZE (20)
# define MINI_WIDTH (180)
# define MINI_X (MINI_WIDTH - (MINI_WIDTH - 15))
# define MINI_Y (HEIGHT - MINI_WIDTH - 10)

# define FPS 80

typedef struct s_text	t_text;
typedef struct s_vector	t_vector;
typedef struct s_rgb	t_rgb;
typedef struct s_image	t_image;
typedef struct s_var	t_var;
typedef struct s_player	t_player;
typedef struct s_win	t_win;
typedef struct s_vect	t_vect;
typedef struct s_key	t_key;

enum					e_key
{
	UP = XK_w,
	DOWN = XK_s,
	LEFT = XK_a,
	RIGHT = XK_d,
	ROT_L = XK_Left,
	ROT_R = XK_Right,
	LOOK_U = XK_Up,
	LOOK_D = XK_Down
};

typedef enum e_cell
{
	wall = '1',
	space = ' ',
	ground = '0',
	p_n = 'N',
	p_s = 'S',
	p_e = 'E',
	p_w = 'W',
	door_c = 'D',
	door_o = 'd',
	sprite = 's'
}						t_cell;

struct					s_vect
{
	int					x;
	int					y;
};

struct					s_vector
{
	double				x;
	double				y;
};

struct					s_rgb
{
	int					r;
	int					g;
	int					b;
};

struct					s_text
{
	char				*no_path;
	char				*so_path;
	char				*we_path;
	char				*ea_path;
	t_rgb				c;
	t_rgb				f;
	char				*ceiling;
	char				*floor;
	t_cell				p;
	bool				error;
};

struct					s_image
{
	void				*img_ptr;
	char				*addr;
	int					bpp;
	int					size_line;
	int					endian;
	int					w;
	int					h;
};

struct					s_key
{
	bool				key_up;
	bool				key_down;
	bool				key_left;
	bool				key_right;
	bool				rot_l;
	bool				rot_r;
	bool				look_up;
	bool				look_down;
	bool				speed;
	bool				recharge;
	bool				hide_mouse;
	bool				key_croutch;
	bool				key_jump;
	bool				scope;
	bool				door_check;
	bool				shoot;
};

struct					s_player
{
	t_vector			pos;
	t_vect				dir;
	t_vector			plan;
	t_key				key;
	double				angle;
	int					turn_dir;
	int					walk_dir;
	int					w;
	int					h;
	int					health;
};

struct					s_var
{
	void				*mlx;
	void				*win;
};

typedef struct s_pack
{
	t_image				arrow;
	t_image				aiming;
	t_image				gun_sp[15];
	t_image				no_img;
	t_image				so_img;
	t_image				ea_img;
	t_image				we_img;
	t_image				floor;
	t_image				ceil;
	t_image				compass;
	t_image				needle;
	t_image				scope;
	t_image				door[2];
	t_image				blood;
	t_image				gov;
	t_image				notice;
	t_image				help;
	t_image				walk[4];
	t_image				death[7];
	t_image				pain;

}						t_pack;

typedef struct s_sprit
{
	t_vector			pos;

	t_vector			d;
	double				p;
	double				q;
	t_vector			sp;
	t_vector			sp_screen;
	double				dist;
	double				proj_sp_height;
	bool				was_shot;
	bool				is_died;
	int					w_frm;
	int					d_frm;
	size_t				d_fps;
	size_t				w_fps;
	int					health;
}						t_sprit;

typedef struct s_door
{
	t_vect				pos;
	double				door_prog;
}						t_door;

typedef struct s_intro
{
	t_image				img[384];
	int					frm;
	size_t				last_time;
}						t_intro;

typedef struct s_game
{
	t_player			player;
	t_text				txt;
	char				**map;
	t_var				var;
	t_image				img;
	t_pack				pack;
	t_sprit				*sp;
	t_door				*doors;
	t_intro				intro;
}						t_game;

typedef struct s_g
{
	pid_t				music_pid;
	bool				one_shoot_music;
	int					j_croutch;
	double				project_dist;
	float				zoom;
	int					narrow_look;
	bool				updated;
	int					door_count;
	int					sp_count;
	bool				game_over;
	bool				help;
}						t_g;

// PARSE
t_text					ft_parsing(char *path_map, char ***game_map);

// PARSE UTILS
int						ft_ispace(char c);
int						ft_skip_space(char *s);
int						ft_array_lenght(char **arr);

// FREE_UTILS
void					*ft_free(void *param);
void					ft_free_arr(void **arr);
void					ft_free_txt(t_text *txt);
void					init_player(t_game *game);

// music
t_g						*g_global(void);
void					stop_music(void);
void					launch_music(char const *file, char *vol);
void					wait_music(void);
size_t					get_current_time(void);
bool					is_music_playing(void);

//
bool					is_wall_door(int x, int y, char **map, t_player player);
double					normalize_angle(double angle);
double					distance_between_points(t_vector a, t_vector b);
bool					is_hit_door(int x, int y, char **map, t_player player);

// door

t_door					*handle_door(char **map);
void					init_door(t_door *door, t_cell cell, int pos_x,
							int pos_y);
int						count_door(char **map);
// void	update_doors(t_game *game, double delta_time);
int						find_door(t_door *doors, int x, int y);
void					check_door_close(t_game *game);

// ray_cast
void					compute_ray_orientation(double angle, bool *up,
							bool *down, bool *left, bool *right);
t_vector				ray_cast_horizontal(t_game *game, double ray_angle,
							bool is_facing_up, bool is_facing_down,
							bool *found_h_o_door, bool *found_horizontal);
t_vector				ray_cast_vertical(t_game *game, double ray_angle,
							bool is_facing_left, bool is_facing_right,
							bool *found_v_o_door, bool *found_vertical);

bool					is_hit_door_open(int x, int y, char **map,
							t_player player);
bool					is_hit_door_close(int x, int y, char **map,
							t_player player);
bool					is_hit_door(int x, int y, char **map, t_player player);

// video
void					load_video(t_game *game);


//free
void	free_all(t_game *game);
#endif