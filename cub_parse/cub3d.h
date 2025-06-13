/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:37:44 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/13 18:31:32 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "./gnl/get_next_line.h"
# include "./libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_text	t_text;
typedef struct s_rgb	t_rgb;

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
};

typedef	struct s_vect
{
	int	x;
	int	y;
}	t_vect;


int						check_arguments(int argc, char **argv);
char					**map_to_char(char *path_map);
int						white_space(char c);
int						white_line(char *str);
char					*get_path(const char *str, char *id_path);
char					*get_rgb(const char *str, char id_rgb);
int						is_valid_rgb_format(const char *str);
int						get_rgb_value(const char *str, int *r, int *g, int *b);
char					**get_map(char **str);
int						around_zero(char c);
int						count_player(char c);
int						validated_map(char **map);
void					init_text(t_text *txt);
void					free_tab(char **tab);
void					free_text(t_text *text);
int						parsing(char **input, t_text *txt, char ***map);
int						check_id_rgb(const char *str, char id_rgb);
int						check_id_path(const char *str, char *id_path);

#endif