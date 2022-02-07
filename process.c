/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:11 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/07 19:05:31 by maabidal         ###   ########.fr       */
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

void	open_file_update_fds(int open_f, char *pathname, int file_stream, int p_fd, t_cmd *cmd)
{
	int	fd;
	int	pipe_stream;

//printf("\n\n pathname = %s\n", pathname);
	pipe_stream = 1 - file_stream;
//printf("pipe stream = %d\n", pipe_stream);
	ft_dup2(p_fd, pipe_stream, cmd);
	fd = ft_open(pathname, open_f, cmd);
	ft_dup2(fd, file_stream, cmd);
}

void	exe_first_proc(char *cmd_s, char *pathname, char **env, int p_write)
{
	t_cmd	cmd;
	int	fd;

	open_file_update_fds(O_RDONLY, pathname, IN, p_write, NULL);
	setup_cmd(&cmd, cmd_s, env);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, *cmd.av, 1);
}

void	exe_last_proc(char *cmd_s, char *pathname, char **env, int p_read, int open_f)
{
	t_cmd	cmd;
	int	fd;

	setup_cmd(&cmd, cmd_s, env);
	open_file_update_fds(open_f, pathname, OUT, p_read, &cmd);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, *cmd.av, 1);
}

void	exe_pipe_proc(char *cmd_s, char **env, int p_read, int p_write)
{
	t_cmd cmd;

	setup_cmd(&cmd, cmd_s, env);
	ft_dup2(p_read, IN, &cmd);
	ft_dup2(p_write, OUT, &cmd);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, cmd.path, 1);
}
