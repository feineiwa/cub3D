/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   music_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 08:38:45 by frahenin          #+#    #+#             */
/*   Updated: 2025/06/15 15:20:51 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d_bonus.h"

bool	is_music_playing(void)
{
	int		status;
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

	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
		execlp("cvlc", "cvlc", "--gain", vol, file, (char *) NULL);
	else
		g_global()->music_pid = pid;
	(void)vol;
}

void	stop_music(void)
{
	if (g_global()->music_pid != -1)
	{
		kill(g_global()->music_pid, SIGTERM);
		g_global()->music_pid = -1;
	}
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_putendl_fd("Error gettimeofday()", 2);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
