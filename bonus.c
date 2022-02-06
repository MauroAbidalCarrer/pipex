/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 18:47:32 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/06 18:38:19 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char *get_next_line(int fd);

int	is_str(char *str1, char *str2)
{
	while (*str1 && *str1 == *str2)
	{
		if (!str1[1] && !str2[1])
			return (1);
		str1++;
		str2++;
	}
	return (0);
}

void	here_doc(int fd, char *limit)
{
	char *line;

	while (1)
	{
		write(OUT, "heredoc>", 8);
		line = get_next_line(IN);
		if (!line || is_str(line, limit))
			break ;
		write(fd, line, index_of(0, line, 0));
	}
}

void	exe_pipe_cmd(char *cmd_s, char **env, int p_read, int p_write)
{
	t_cmd cmd;

	setup_cmd(&cmd, cmd_s, env);
	if(dup2(p_read, IN) == -1 || dup2(p_write, OUT) == -1)
		exit_with_error(&cmd, NULL, NULL, 1);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd, NULL, cmd.path, 1);
}

void	exe_mid_cmds(int nb_cmds, char **av, char **env, int *p_fds1)
{
	int	child_pid;
	int	p_fds2[2];

	while (nb_cmds--)
	{
		if (pipe(p_fds2) == -1)
			exit_with_error(NULL, NULL, NULL, 1);
		child_pid = fork();
		if (child_pid == -1)
			exit_with_error(NULL, NULL, NULL, 1);
		if (child_pid == 0)
			exe_pipe_cmd(*(av++), env, p_fds1[0], p_fds2[1]);
		p_fds1 = p_fds2;
	}
}

//pour heredoc, juste pipe puis erite sur write avec gnl
int	 main(int ac, char **av, char **env)
{
	int	hd;
	int	child_pid;
	int	p_fds[2];

	if (ac < 5)
	{
		write(2, "nb of args incorrect, write at least 4 args\n", 43);
		return (1);
	}
	if (pipe(p_fds) == -1)
		exit_with_error(NULL, NULL, NULL, 1);
	hd = is_str("here_doc", av[1]);
	if (hd)
		here_doc(p_fds[1], av[2]);
	av += 1 + hd * 2;
	child_pid = fork();
	if (child_pid == -1)
		exit_with_error(NULL, NULL, NULL, 1);
	if (child_pid == 0)
	{	
		if (hd)
			exe_pipe_cmd(*av, env, p_fds[0], p_fds[1]);
		else
			exe_first_cmd(av[1], av[0], env, p_fds);
	}
	exe_mid_cmds(ac - 5 - hd, av + 1 + hd, env, p_fds);
	if (hd)
		exe_last_cmd(av[ac - 4 - hd], av[ac - 3 - hd], env, p_fds, APPEND_F);
	else
		exe_last_cmd(av[ac - 4 - hd], av[ac - 3 - hd], env, p_fds, CREAT_F);
}
/*
	int	p_fds[2];
	int	child_pid;

	if (ac != 5)
	{
		write(2, "pipex: Error: nb of arguments isn't 4\n", 38);
		return (1);
	}
	if (pipe(p_fds) == -1)
		exit_with_error(NULL, NULL, NULL, 1);
	child_pid = fork();
	if (child_pid == -1)
		exit_with_error(NULL, NULL, NULL, 1);
	if (child_pid == 0)
		exe_first_cmd(av[2], av[1], env, p_fds);
	exe_last_cmd(av[3], av[4], env, p_fds);
*/
