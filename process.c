/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:11 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/04 21:26:38 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	exit_with_error(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->not_found)
		{
			write(2, "zsh: command not found: ", 24);
			write(2, *cmd->av, index_of(0, *cmd->av, 0));
			write(2, "\n", 1);
		}
		while (--cmd->ac >= 0)
			free(cmd->av[cmd->ac]);
		free(cmd->av);
		if (cmd->path)
			free(cmd->path);
		if (cmd->not_found)
			exit(127);
	}
	if (!cmd || !cmd->dont_print_msg)
		perror("zsh");
	exit(1);
}

int	open_file(char *pathname, int flags, int c_mode, t_cmd *cmd)
{
	int	fd;

	fd = open(pathname, flags, c_mode);
	if (fd == -1)
	{
		exit_status = F_PERM_DEN;
		if (errno == 2)
			exit_status = F_NOT_FOUND;
		exit_with_error(cmd, file, exit_status);
	}
	return (fd);
}

void	update_fds(int fd, int std_fd, int std_p, int p_use, int p_close)
{
	if (fd != std_stream)
	{
		if (dup2(fd, std_fd) == -1)
			exit_with_error(NULL, NULL, 1);
	}
	if (close(p_close) == -1 || dup2(p_use, std_p) == -1)
		exit_with_error(NULL);
}

void	fisrt_cmd(char *cmd_s, char *file, char **env, int p_fds[2])
{
	t_cmd	cmd;
	int	fd;

	fd = open_file(file, O_RDONLY, 0, NULL);
	update_fds(fd, IN, OUT, p_fds[1], p_fds[0]);
	setup_cmd(&cmd, cmd_s);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, 1);
}

void	last_cmd(char *cmd_s, char *file, char **env, int p_fds[2])
{
	t_cmd	cmd;
	int	fd;

	setup_cmd(&cmd, cmd_s);
	fd = open_file(file, C_FLAGS, C_MODE, cmd)
	update_fds(fd, OUT, IN, p_fds[0], p_fds[1]);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, 1);
}
