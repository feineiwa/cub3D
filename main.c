/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:07:36 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/18 12:54:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

pid_t	music_pid = -1;

void	put_pixel(int x, int y, int color, t_img *img)
{
	// char	*offset;
	int	offset;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	offset = (y * img->line_len) + (x * img->bpp / 8);
	img->px_ptr[offset] = color & 0xFF;
	img->px_ptr[offset + 1] = (color >> 8) & 0xFF;
	img->px_ptr[offset + 2] = (color >> 16) & 0xFF;
// 	offset = img->px_ptr + (y * img->line_len + x * (img->bpp / 8));
// 	*(int *)offset = color;
}

void	draw_square(int x, int y, int size, int color, t_game *game)
{
	int	i;
	int	j;

	j = 0;
	while (j < size)
	{
		i = 0;
		while (i < size)
		{
			put_pixel(x + i, y + j, color, &game->vars.img);
			i++;
		}
		j++;
	}
	// while (i < size)
	// {
	// 	put_pixel(x + i, y, color, game);
	// 	i++;
	// }
	// i = 0;
	// while (i < size)
	// {
	// 	put_pixel(x, y + i, color, game);
	// 	i++;
	// }
	// i = 0;
	// while (i < size)
	// {
	// 	put_pixel(x + size, y + i, color, game);
	// 	i++;
	// }
	// i = 0;
	// while (i < size)
	// {
	// 	put_pixel(x + i, y + size, color, game);
	// 	i++;
	// }
}

void	draw_map(t_game *game)
{
	char	**map;
	int		x;
	int		y;
	int		color;

	map = game->map;
	color = 0x0000FF;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == wall)
				draw_square(x * 32, y * 32, 30, color, game);
			x++;
		}
		y++;
	}
}

int	get_img_color(t_img *img, int x, int y)
{
	return (*(int *)(img->px_ptr + ((y * img->line_len) + (x * (img->bpp
						/ 8)))));
}

void	paint_image_to_win(t_game *game, t_img *img, char *path, int s_x,
		int s_y)
{
	t_img	img1;
	int		x;
	int		y;
	int		img_x;
	int		img_y;
	int		color;

	img1.img_ptr = mlx_xpm_file_to_image(game->vars.mlx, path, &img_x, &img_y);
	img1.px_ptr = mlx_get_data_addr(img1.img_ptr, &img1.bpp, &img1.line_len,
			&img1.endian);
	y = 0;
	while (y < s_y)
	{
		x = 0;
		while (x < s_x)
		{
			color = get_img_color(&img1, (int)((double)x * ((double)img_x
							/ (double)s_x)), (int)((double)y * ((double)img_y
							/ (double)s_y)));
			if (color == 0)
				color = 0x0000FF;
			put_pixel(x, y, color, img);
			x++;
		}
		y++;
	}
	mlx_destroy_image(game->vars.mlx, img1.img_ptr);
}

void	clear_image(t_img *img, int w, int h)
{
	int	x;
	int	y;
	
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			put_pixel(x, y, 0, img);
			x++;
		}
		y++;
	}
}

int	touch(double py, double px, t_game *game)
{
	int	x;
	int	y;

	x = (int)floor(px);
	y = (int)floor(py);
	if (game->map[y][x] == '1')
		return (1);
	return (0);
}

int	draw_loop(t_game *game)
{
	// double	speed;
	// double	angle_speed;
	// double	cos_angle;
	// double	sin_angle;
	// double	ray_x;
	// double	ray_y;
	
	// speed = 0.02;
	// angle_speed = 0.02;
	// ray_x = game->player.x;
	// ray_y = game->player.y;
	clear_image(&game->vars.img, WIDTH, HEIGHT);
	// clear_image(&game->p_img, 15, 15);
	if (game->flag_video_is_running == 1)
	{
		if (game->current_frame < VIDEO_FRAME)
		{
			mlx_clear_window(game->vars.mlx, game->vars.win);
			mlx_put_image_to_window(game->vars.mlx, game->vars.win, game->frames[game->current_frame], 0, 0);
			game->current_frame++;
			usleep(80000);
		}
		else
		game->flag_video_is_running = 0;
		return (0);
	}
	mlx_clear_window(game->vars.mlx, game->vars.win);
	int	x;
	double	cam_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	x = 0;
	while (x < WIDTH)
	{
		cam_x = 2 * x / (double)WIDTH - 1;
		ray_dir_x = game->dir.x  + game->plan.x * cam_x;
		ray_dir_y = game->dir.y  + game->plan.y * cam_x;
		map_x = (int)game->player.x;
		map_y = (int)game->player.y;
		delta_dist_x = fabs(1 / ray_dir_x);
		delta_dist_y = fabs(1 / ray_dir_y);
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (game->player.x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - game->player.x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (game->player.y - map_y) * delta_dist_y;
		}
		else
		{
			step_x = 1;
			side_dist_y = (map_y + 1.0 - game->player.y) * delta_dist_y;
		}
		while (map_x >= 0 && map_x < game->width &&
       		map_y >= 0 && map_y < game->height &&
       		game->map[map_y][map_x] == 'N')
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
			}
		}
		int line_height = (int)(HEIGHT / (side_dist_x < side_dist_y ? side_dist_x : side_dist_y));
		int	draw_start = -line_height / 2 + HEIGHT / 2;
		int	draw_end = line_height / 2 + HEIGHT / 2;
		int y = draw_start;
		while (y < draw_end)
		{
			put_pixel(x, y, 0x0000FF, &game->vars.img);
			y++;
		}
		x++;
	}
	// if (game->rotate_left == true)
	// 	game->angle += angle_speed;
	// if (game->rotate_right == true)
	// 	game->angle -= angle_speed;
	// cos_angle = cos(game->angle);
	// sin_angle = sin(game->angle);
	// if (game->angle > 2 * MYPI)
	// game->angle = 0;
	// if (game->angle < (double)0)
	// 	game->angle = 2 * MYPI;
	// while (touch(ray_y, ray_x, game) == 0)
	// {
	// 	put_pixel(ray_x * 32, ray_y * 32, 0xFF0000, &game->vars.img);
	// 	ray_x += cos_angle * speed;
	// 	ray_y -= sin_angle * speed;
	// }
	// if (game->key_up == true && game->player.x != ray_x && game->player.y != ray_y)
	// {
	// 	game->player.x += cos_angle * speed;
	// 	game->player.y -= sin_angle * speed;
		
	// }
	// if (game->key_down == true)
	// {
	// 	game->player.x += cos_angle * speed;
	// 	game->player.y += sin_angle * speed; 
	// }
	// if (game->key_right == true)
	// {
	// 	game->player.x += sin_angle * speed;
	// 	game->player.y -= cos_angle * speed;
	// }
	// if (game->key_left == true)
	// {
	// 	game->player.x -= sin_angle * speed;
	// 	game->player.y += cos_angle * speed;
	// }
	// draw_map(game);
	mlx_put_image_to_window(game->vars.mlx, game->vars.win,
		game->vars.img.img_ptr, 0, 0);
		// paint_image_to_win(game, &game->p_img, "4.xpm", 15, 15);
	// mlx_put_image_to_window(game->vars.mlx, game->vars.win, game->p_img.img_ptr,
		// game->player.x * 32, game->player.y * 32);
	return (0);
}

void	wait_music(void)
{
	pid_t	res;

	if (music_pid != -1)
	{
		res = waitpid(music_pid, 0, WNOHANG);
		if (res == music_pid)
			music_pid = -1;
	}
	music_pid = -1;
}

void	launch_music(char const *file, char *vol)
{
	pid_t	pid;
	// char *sound;
	// char back[2];
	// char	*last;

	// sound = ft_strjoin("cvlc ", file);
	// back[0] = ' ';
	// back[1] = '&';
	// last = ft_strjoin(sound, back);
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
	{
		execlp("cvlc", "cvlc", "--gain", vol, file, (char *)NULL);
		// execlp("cvlc", "cvlc", "--volume", "5", file, (char *)NULL);
		// execlp("ffplay", "ffplay", "-volume", "50", "-nodisp", "-autoexit", file, (char *)NULL);
		// execlp("mpv", "mpv", "--volume=50", "--no-video", file, (char *)NULL);
	}
	else
		music_pid = pid;
	// system(last);
	// sleep(1);
	// ft_free(sound);
	// ft_free(last);
}

void	stop_music(void)
{
	if (music_pid != -1)
	{
		kill(music_pid, SIGTERM);
		music_pid = -1;
	}
}

// int	key_press(int keysym, t_game *game)
// {
// 	if (keysym == XK_f && game->flag_video_is_running == 0)
// 	{
// 		launch_music("gun-shot-sound-effect-224087.wav", "0.5");
// 	}
// 	else if (keysym == XK_space && game->flag_video_is_running == 1)
// 	{
// 		stop_music();
// 		game->flag_video_is_running = 0;
// 	}
// 	return (0);
// }

int	move_player_keypress(int keysym, t_game *game)
{
	if (keysym == XK_w && !game->flag_video_is_running)
		game->key_up = true;
	else if (keysym == XK_d && !game->flag_video_is_running)
		game->key_right = true;
	else if (keysym == XK_a && !game->flag_video_is_running)
		game->key_left = true;
	else if (keysym == XK_s && !game->flag_video_is_running)
		game->key_down = true;
	else if (keysym == XK_Left && !game->flag_video_is_running)
		game->rotate_left = true;
	else if (keysym == XK_Right && !game->flag_video_is_running)
		game->rotate_right = true;
	if (keysym == XK_f && game->flag_video_is_running == 0 && game->gun_shooting == false)
	{
		game->gun_shooting = true;
		launch_music("gun-shot-sound-effect-224087.wav", "0.5");
	}
	else if (keysym == XK_space && game->flag_video_is_running == 1)
	{
		stop_music();
		game->flag_video_is_running = 0;
	}
	return (0);
}

int	move_player_keyrelease(int keysym, t_game *game)
{
	if (keysym == XK_w && !game->flag_video_is_running)
		game->key_up = false;
	else if (keysym == XK_d && !game->flag_video_is_running)
		game->key_right = false;
	else if (keysym == XK_a && !game->flag_video_is_running)
		game->key_left = false;
	else if (keysym == XK_s && !game->flag_video_is_running)
		game->key_down = false;
	else if (keysym == XK_Left && !game->flag_video_is_running)
		game->rotate_left = false;
	else if (keysym == XK_Right && !game->flag_video_is_running)
		game->rotate_right = false;
	if (keysym == XK_f && game->flag_video_is_running == 0 && game->gun_shooting == true)
		game->gun_shooting = false;
	return (0);
}

void	load_video(t_game *game)
{
	char	filename[30];
	int		i;
	int		j;

	ft_memcpy((char *)filename, (char *)"../video/000.xpm", 17);
	i = 0;
	while (i < VIDEO_FRAME)
	{
		j = i + 1;
		if (j >= 10 && j <= 99)
		{
			filename[10] = '0' + (j / 10);
			filename[11] = '0' + (j % 10);
		}
		else if (j > 99)
		{
			filename[9] = '0' + ((j / 10) / 10);
			filename[10] = '0' + ((j / 10) % 10);
			filename[11] = '0' + (j % 10);
		}
		else
			filename[11] = '0' + j;
		game->frames[i] = mlx_xpm_file_to_image(game->vars.mlx, filename, &j, &j);
		if (!game->frames[i])
			return ;
		i++;
	}
}


int	main(int ac, char **av)
{
	t_game	*game;
	t_img	img;

	if (ac != 2)
	{
		printf("Error\nArguments should be just 2\n");
		return (1);
	}
	ft_parsing(av[1]);
	game = data_handle();
	init_player(game);
	// ft_free_txt(&game.txt);
	// ft_free_arr((void **)game.map);
	game->vars.mlx = mlx_init();
	game->vars.win = mlx_new_window(game->vars.mlx, WIDTH, HEIGHT, "Cub3D");
	img.img_ptr = mlx_new_image(game->vars.mlx, WIDTH, HEIGHT);
	img.px_ptr = mlx_get_data_addr(img.img_ptr, &img.bpp, &img.line_len,
			&img.endian);
	game->vars.img = img;
	game->p_img.img_ptr = mlx_new_image(game->vars.mlx, 15, 15);
	game->p_img.px_ptr = mlx_get_data_addr(game->p_img.img_ptr,
			&game->p_img.bpp, &game->p_img.line_len, &game->p_img.endian);
	mlx_put_image_to_window(game->vars.mlx, game->vars.win,
		game->vars.img.img_ptr, 0, 0);
	// mlx_put_image_to_window(game->vars.mlx, game->vars.win,
	// 	game->p_img.img_ptr, game->player.x * 32, game->player.y * 32);
	// stop_music();
	game->current_frame = 0;
	game->flag_video_is_running = 1;
	load_video(game);
	// launch_music("faneva.wav", "1.0");
	mlx_hook(game->vars.win, KeyPress, KeyPressMask, move_player_keypress, game);
	// mlx_key_hook(game->vars.win, key_press, game);
	mlx_hook(game->vars.win, KeyRelease, KeyReleaseMask, move_player_keyrelease, game);
	draw_loop(game);
	mlx_loop_hook(game->vars.mlx, draw_loop, game);
	mlx_loop(game->vars.mlx);
	return (0);
}
