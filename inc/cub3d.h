/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:56:54 by frahenin          #+#    #+#             */
/*   Updated: 2025/04/01 19:11:36 by frahenin         ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef BONUS
#  define BONUS 0
# endif

// # define WIDTH 1200
// # define HEIGHT 800
# define PI 3.14159265359
# define MOVE_SPEED 0.1
# define ROT_SPEED ((2 * PI) / 180)
# define RES 1
# define TILE_SIZE 32
# define ROWS 14
# define COLS 33
# define WIDTH (COLS * TILE_SIZE)
# define HEIGHT (ROWS * TILE_SIZE)
# define FOV 1.0471975512
# define R_NB WIDTH

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
};

struct					s_key
{
	bool				key_up;
	bool				key_down;
	bool				key_left;
	bool				key_right;
	bool				rot_l;
	bool				rot_r;
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
};

struct					s_var
{
	void				*mlx;
	void				*win;
};

typedef struct s_game
{
	t_player			player;
	t_text				txt;
	char				**map;
	t_var				var;
	t_image				img;
}						t_game;

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
#endif