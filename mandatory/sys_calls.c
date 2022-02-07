/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 15:45:28 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/07 20:38:45 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_close(int fd)
{
	if (close(fd) == -1)
		exit_with_error(NULL, NULL, NULL, 1);
}

int	ft_open(char *pathname, int open_fs, t_cmd *cmd)
{
	int	fd;

	fd = open(pathname, open_fs, CREAT_M);
	if (fd == -1)
		exit_with_error(cmd, NULL, pathname, 1);
	return (fd);
}

void	ft_dup2(int old_fd, int new_fd, t_cmd *cmd)
{
	if (dup2(old_fd, new_fd) == -1)
		exit_with_error(cmd, NULL, NULL, 1);
}

int	ft_fork(void)
{
	int	child_pid;

	child_pid = fork();
	if (child_pid == -1)
		exit_with_error(NULL, NULL, NULL, 1);
	return (child_pid);
}

void	ft_pipe(int *p_fds)
{
	if (pipe(p_fds) == -1)
		exit_with_error(NULL, NULL, NULL, 1);
}
