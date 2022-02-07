/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 18:47:32 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/07 15:08:44 by maabidal         ###   ########.fr       */
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
	char	*line;
	char	tmp;

	while (1)
	{
		write(OUT, "heredoc> ", 9);
		line = get_next_line(IN);
		if (!line)
			break ;
		tmp = line[index_of(0, limit, 0)];
		if (tmp == '\n')
			line[index_of(0, limit, 0)] = 0;
//printf("limit = \"%s\"\n", limit);
//printf("line = \"%s\"\n", line);
		if (is_str(line, limit))
			break ;
		line[index_of(0, limit, 0)] = tmp;
		write(fd, line, index_of(0, line, 0));
	}
}

void	exe_mid_cmds(int nb_cmds, char **av, char **env, int *p_fds1)
{
	int	child_pid;
	int	p_fds2[2];

	while (nb_cmds--)
	{
printf("nb_cmds = %d\n", nb_cmds);
printf("cmd = \"%s\", closing p_fds[1] = %d\n", *av, p_fds1[1]);
		if (close(p_fds1[1]) == -1)
			exit_with_error(NULL, NULL, NULL, 1);
		if (pipe(p_fds2) == -1)
			exit_with_error(NULL, NULL, NULL, 1);
printf("cmd = \"%s\", pipe(p_fds2) p_fds2[0] = %d, p_fds2[1] = %d\n", *av, p_fds2[0], p_fds2[1]);
		child_pid = fork();
		if (child_pid == -1)
			exit_with_error(NULL, NULL, NULL, 1);
printf("child_pid = %d\n", child_pid);
		if (child_pid == 0)
{
//printf("cmd %d, cmd_s = \"%s\"\n", nb_cmds, *av);
			if (close(p_fds2[0]) == -1)
				exit_with_error(NULL, NULL, NULL, 1);
			exe_pipe_cmd(*av, env, p_fds1[0], p_fds2[1]);
}
av++;
//printf("done\n");
//printf("cmd = \"%s\", assigning p_fds2[0] = %d to p_fds1[0] = %d\n", *av, p_fds2[0], p_fds1[0]);
		p_fds1[0] = p_fds2[0];
//printf("cmd = \"%s\", closing %d\n", *av, p_fds2[1]);
		if (close(p_fds2[1]) == -1)
			exit_with_error(NULL, NULL, NULL, 1);
		//p_fds1 = p_fds2;
//printf("\n\n");
	}
//printf("done\n");
}

void	exe_cmds(int ac, char **av, char **env, int hd, int *p_fds)
{
	int	child_pid;
	int	p_fds2[2];

	child_pid = fork();
	if (hd)
		if (pipe(p_fds2) == -1)
			exit_with_error(NULL, NULL, NULL, 1);
	if (child_pid == -1)
		exit_with_error(NULL, NULL, NULL, 1);
	if (child_pid == 0)
	{
		if (hd )
{
//printf("first cmd_s = \"%s\"\n", *av);
			exe_pipe_cmd(*av, env, p_fds[0], p_fds2[1]);
}
		else
		{
//printf("first cmd_s = \"%s\", file = \"%s\"\n", av[1], av[0]);
			exe_first_cmd(av[1], av[0], env, p_fds);
		}
	}
	if (hd)
	{
		printf("closing p_fds[0] = %d\n", p_fds[0]);
		if(close(p_fds[0]) == -1)
			exit_with_error(NULL, NULL, NULL, 1);
		if (ac - 6 == 0)
		{
			printf("closing p_fds[1] = %d\n", p_fds[1]);
			if(close(p_fds[1]) == -1)
				exit_with_error(NULL, NULL, NULL, 1);
		}
		else
			printf("nope\n");
printf("assigning p_fds[0] = %d,  to p_fds2[0] = %d\n", p_fds[0], p_fds2[0]);
		p_fds[0] = p_fds2[0];
	}
//printf("p_fds[0] = %d\n", p_fds[0]);
//printf("nb mid cmds = %d\n", ac - 5 - hd);
//printf("first mid cmds = %s\n", av[2 - hd]);
	exe_mid_cmds(ac - 5 - hd, av + 2 - hd, env, p_fds);
//printf("p_fds[0] = %d\n", p_fds[0]);
//printf("executing last cmd\n");
	exe_last_cmd(av[ac - 3 - hd * 2], av[ac - 2 - hd * 2], env, p_fds, hd);
}

int	 main(int ac, char **av, char **env)
{
	int	hd;
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
	exe_cmds(ac, av, env, hd, p_fds);
}
