/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:11 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/06 18:51:55 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

//have to malloc error_msgs to print it all at once
void	exit_with_error(t_cmd *cmd, char *c_msg, char *app_msg, int exit_status)
{
	char	buff[2048];

	if (!c_msg)
	{
		str_cat(BEF_ERR, buff);
		str_cat(strerror(errno), buff);
	}
	else
		str_cat(c_msg, buff);
	if (app_msg)
	{
		str_cat(": ", buff);
		str_cat(app_msg, buff);
	}
	str_cat("\n", buff);
	write(2, buff, index_of(0, buff, 0));
	if (cmd)
	{
		while (--cmd->ac >= 0)
			free(cmd->av[cmd->ac]);
		free(cmd->av);
		if (cmd->path)
			free(cmd->path);
	}
	exit(exit_status);
}

int	open_file(char *pathname, int flags, t_cmd *cmd)
{
	int	fd;

	fd = open(pathname, flags, CREAT_M);
	if (fd == -1)
		exit_with_error(cmd, NULL, pathname, 1);
	return (fd);
}

void	update_fds(int fd, int std_fd, int std_p, int p_use, int p_close, t_cmd *cmd)
{
	if (fd != std_fd)
	{
		if (dup2(fd, std_fd) == -1)
			exit_with_error(cmd, null, null, 1);
	}
	if (close(p_close) == -1 || dup2(p_use, std_p) == -1)
		exit_with_error(cmd, null, null, 1);
}

void	open_file_update_fds(int flags, int std_fd, int std_p, int p_use, int p_close, t_cmd *cmd)
{
	int	fd;

	fd = open(pathname, flags, CREAT_M);
	if (fd == -1)
		exit_with_error(cmd, NULL, pathname, 1);
	if (fd != std_fd)
	{
		if (dup2(fd, std_fd) == -1)
			exit_with_error(cmd, null, null, 1);
	}
	if (close(p_close) == -1 || dup2(p_use, std_p) == -1)
		exit_with_error(cmd, null, null, 1);
}

void	exe_first_cmd(char *cmd_s, char *file, char **env, int p_fds[2])
{
	t_cmd	cmd;
	int	fd;

	fd = open_file(file, O_RDONLY, NULL);
	update_fds(fd, IN, OUT, p_fds[1], p_fds[0], &cmd);
	setup_cmd(&cmd, cmd_s, env);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, *cmd.av, 1);
}

void	exe_last_cmd(char *cmd_s, char *file, char **env, int p_fds[2], int flags)
{
	t_cmd	cmd;
	int	fd;


	setup_cmd(&cmd, cmd_s, env);
	fd = open_file(file, flags, &cmd);
	update_fds(fd, OUT, IN, p_fds[0], p_fds[1], &cmd);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, NULL, 1);
}
