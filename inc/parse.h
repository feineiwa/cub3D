/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 00:37:44 by eralambo          #+#    #+#             */
/*   Updated: 2025/06/16 14:45:30 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "type.h"

int		check_arguments(int argc, char **argv);
char	**map_to_char(char *path_map);
int		white_space(char c);
int		white_line(char *str);
char	*get_path(const char *str, char *id_path);
char	*get_rgb(const char *str, char id_rgb);
int		is_valid_rgb_format(const char *str);
int		get_rgb_value(const char *str, int *r, int *g, int *b);
int		around_zero(char c);
int		count_player(char c);
int		validated_map(char **map);
void	init_text(t_text *txt);
void	*ft_free(void *param);
void	ft_free_arr(void **arr);
void	ft_free_txt(t_text *txt);
void	ft_free_parse(char **input, t_text *txt);
int		parse_input(char **input, t_text *txt, char ***map);
int		check_id_rgb(const char *str, char id_rgb);
int		check_id_path(const char *str, char *id_path);
void	padding_space(char **map);
void	replace_new_line_space(char *s);
int		ft_array_lenght(char **arr);
int		parsing(char *av, char ***map, t_text *txt);
int		is_validated_color(t_text *txt);
char	**append_line(char **lines, int count, char *new_line);
char	**optimisation(char **map);

#endif