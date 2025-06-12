/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 22:08:55 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/12 22:10:57 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_H
# define TYPE_H

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

# define PI 3.14159265359
# define MOVE_SPEED 0.04
# define ROT_SPEED 0.05
# define MOUSE_SPEED 0.0005
# define RES 1
# define TILE_SIZE 32
# define WIDTH 1600
# define HEIGHT 960
# define HALF_HEIGHT 480
# define HALF_WIDTH 800
# define FOV 1.0471975512
# define HALF_FOV 0.523598776
# define R_NB 1600
# define MINI_SIZE 20
# define MINI_WIDTH 180

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
	t_vector			plan;
	t_key				key;
	double				angle;
	int					turn_dir;
	int					walk_dir;
	int					walk_dir_side;
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
	t_image				load;

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

typedef struct s_r
{
	t_vector			a;
	t_vector			s;
	t_vector			horiz_p;
	t_vector			vert_p;
	t_vector			wall_hit;
	bool				is_facing_down;
	bool				is_facing_left;
	bool				is_facing_right;
	bool				is_facing_up;
	bool				found_horizontal;
	bool				found_vertical;
	double				ray_angle;
	int					ray;
	double				middle;
	double				ray_dist[R_NB];
	double				waves;
	double				horizontal_dist;
	double				vertical_dist;
	double				dist_wall;
	double				o;
	int					offset;
	double				line_height;
	double				draw_begin;
	double				draw_end;
	double				adjusted_fov;
}						t_r;

typedef struct s_d
{
	t_vector			door_v;
	t_vector			door_h;
	double				door_v_dist;
	double				door_h_dist;
	int					door_index;
}						t_d;

typedef struct s_squ
{
	double				angle;
	t_vector			rotated;
	t_vector			relative;
	t_vect				draw;
	t_vector			c;
	t_vect				start;
	t_vect				end;
	t_vect				tex;
	double				cos_a;
	double				sin_a;
	t_vect				d;
	t_vector			r;
	double				scale;
}						t_squ;

typedef	struct s_floor
{
	int			color;
	double		cos_ray;
	double		sin_ray;
	double		distance;
	double		diff;
	double		s;
	int			tex_x;
	int			tex_y;
	double		tile_size_inv;
	double		beta;
	t_vector	f;
	int			start;
	int			end;
}		t_floor;


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
	bool				bonus;
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
	int					gun_frm;
	float				look;
	int					jump;
	bool				first;
	size_t				door_time;
	int					prev_index;
}						t_g;

# endif