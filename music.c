/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   music.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 08:38:45 by frahenin          #+#    #+#             */
/*   Updated: 2025/05/14 09:24:40 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/cub3d.h"

bool	is_music_playing(void)
{
	int	status;
	pid_t	res;
	
	if (g_global()->music_pid == -1)
		return (false);
	res = waitpid(g_global()->music_pid, &status, WNOHANG);
	if (res == g_global()->music_pid)
		return (g_global()->music_pid = -1, false);
	else if (res == 0)
		return (true);
	else
		return (g_global()->music_pid = -1, false);
}

void	wait_music(void)
{
	pid_t	res;

	if (g_global()->music_pid != -1)
	{
		res = waitpid(g_global()->music_pid, 0, WNOHANG);
		if (res == g_global()->music_pid)
			g_global()->music_pid = -1;
	}
	g_global()->music_pid = -1;
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
		g_global()->music_pid = pid;
	// system(last);
	// sleep(1);
	// ft_free(sound);
	// ft_free(last);
}

void	stop_music(void)
{
	if (g_global()->music_pid != -1)
	{
		kill(g_global()->music_pid, SIGTERM);
		g_global()->music_pid = -1;
	}
}

t_g	*g_global(void)
{
	static t_g	value = {-1, false, 0, 0.f, 1.0f, 875, false, 0};
	return (&value);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("Error gettimeofday()\n");
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}