/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:56:54 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/18 16:16:06 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdbool.h>
# include <unistd.h>
# include <math.h>
// # include "./libft/libft.h"
// # include "./gnl/get_next_line.h"
# include "../mlx/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

# ifndef BONUS
#  define BONUS 0
# endif

# define WIDTH 1200
# define HEIGHT 800
# define IMAGE_SIZE	20
# define PI 3.14159265359
# define MOVE_SPEED 0.2
# define ROT_SPEED 0.1

typedef struct s_text	t_text;
typedef struct s_vector	t_vector;
typedef struct s_rgb	t_rgb;
typedef struct s_image	t_image;
typedef struct s_var	t_var;


enum	e_key
{
	UP = XK_w,
	DOWN = XK_s,
	LEFT = XK_a,
	RIGHT = XK_d,
	ROT_L = XK_Left,
	ROT_R = XK_Right,
	LOOK_U = XK_Up,
	LOOK_D = XK_Down,
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
	char				p;
	bool				error;
};


struct					s_image
{
	void				*img_ptr;
	char				*addr;
	int					bpp;
	int					size_len;
	int					endian;
};

typedef struct s_base_vector
{
	t_vector e_x;
	t_vector e_y;
} t_base_vector;

struct s_player
{
	t_vector	pos;
	t_vector	dir;
};


struct s_var
{
	void	*mlx;
	void	*win;
};

struct s_window
{
	t_var	var;
	t_image	mini_map;
};

typedef struct s_cub
{
	t_parse	parse;
	t_cell	**map;
	t_window window;
}	t_cub;

#endif