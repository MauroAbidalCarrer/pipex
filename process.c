/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:11 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/07 15:07:36 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

//have to malloc error_msgs to print it all at once
void	exit_with_error(t_cmd *cmd, char *c_msg, char *app_msg, int exit_status)
{
	char	buff[2048];

	if (!c_msg)
	{
		cat_error_msg(BEF_ERR, buff);
		cat_error_msg(strerror(errno), buff);
	}
	else
		cat_error_msg(c_msg, buff);
	if (app_msg)
	{
		cat_error_msg(": ", buff);
		cat_error_msg(app_msg, buff);
	}
	cat_error_msg("\n", buff);
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

void	open_file_update_fds(int open_f, char *pathname, int fd_flags, int *p_fds, t_cmd *cmd)
{
	int	fd;
	int	pipe_index;//index of pipe to replace pipe_stream

	fd = open(pathname, open_f, CREAT_M);
	if (fd == -1)
		exit_with_error(cmd, NULL, pathname, 1);
	pipe_index = !(fd_flags & 1);
	printf("pathname = \"%s\", fd_flags = %d, pipe_index = %d, p_fds[pipe_index] = %d ", pathname, fd_flags, pipe_index, p_fds[pipe_index]);
	printf("p_fds[1 - pipe_index] = %d\n", p_fds[1 - pipe_index]);
	if (fd_flags & C_PIPE_FD)
	{
		printf("closing p_fds[1 - pipe_index] = %d\n", p_fds[pipe_index]);
		if (close(p_fds[1 - pipe_index]) == -1)
			exit_with_error(cmd, NULL, NULL, 1);
//		printf("called\n");
		printf("closed p_fds[1 - pipe_index] = %d\n", p_fds[pipe_index]);
	}
	else
		printf("not called\n");
	//printf("opend file = \"%s\", dupe2(%d, %d) done\n", pathname, fd, file_stream);
	//printf("dupe2(%d, %d) done\n", p_fds[pipe_index], pipe_index);
	if (dup2(fd, fd_flags & 1) == -1)
		exit_with_error(cmd, NULL, NULL, 1);
	if (dup2(p_fds[pipe_index], pipe_index) == -1)
//	{
//		printf("stopped\n");
		exit_with_error(cmd, NULL, NULL, 1);
//	}
}

void	exe_first_cmd(char *cmd_s, char *pathname, char **env, int p_fds[2])
{
	t_cmd	cmd;
	int	fd;

	open_file_update_fds(O_RDONLY, pathname, IN, p_fds, NULL);
	setup_cmd(&cmd, cmd_s, env);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, *cmd.av, 1);
}

void	exe_last_cmd(char *cmd_s, char *pathname, char **env, int p_fds[2], int hd)
{
	t_cmd	cmd;
	int	fd;

	setup_cmd(&cmd, cmd_s, env);
	if (hd)
		open_file_update_fds(APPEND_F, pathname, OUT, p_fds, &cmd);
	else
		open_file_update_fds(CREAT_F, pathname, OUT | C_PIPE_FD, p_fds, &cmd);
//printf("called3\n");
	execve(cmd.path, cmd.av, env);
//printf("called4\n");
	exit_with_error(&cmd, NULL, *cmd.av, 1);
}

void	exe_pipe_cmd(char *cmd_s, char **env, int p_read, int p_write)
{
	t_cmd cmd;

	setup_cmd(&cmd, cmd_s, env);
	if(dup2(p_read, IN) == -1 || dup2(p_write, OUT) == -1)
		exit_with_error(&cmd, NULL, NULL, 1);
//printf("cmd_s = %s\n", cmd_s);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, cmd.path, 1);
}
