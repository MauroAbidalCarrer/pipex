/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 18:47:32 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/07 19:05:13 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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
		if (is_str(line, limit))
			break ;
		line[index_of(0, limit, 0)] = tmp;
		write(fd, line, index_of(0, line, 0));
	}
	ft_close(fd);
}

int	exe_mid_cmds(int nb_cmds, char **av, char **env, int c_read_p)
{
	int	p_fds2[2];

	while (nb_cmds--)
	{
		ft_pipe(p_fds2);			//c_read_p, p_fds2
//printf("cmd = \"%s\", ", *av);
//printf("piped(p_fds2), c_read_p = %d, p_fds2[0] = %d, p_fds2[1] = %d\n", c_read_p, p_fds2[0], p_fds2[1]);
		if (ft_fork() == 0)
		{
			ft_close(p_fds2[0]);		//c_read_p, p_fds2[1]
			exe_pipe_proc(*av, env, c_read_p, p_fds2[1]);
		}
		ft_close(p_fds2[1]);			//p_fds2[0], p_fds2[1]
		ft_close(c_read_p);			//p_fds2[0]
		c_read_p = p_fds2[0];			//c_read_p
		av++;
	}
//printf("done, c_read_p = %d\n", c_read_p);
	return (c_read_p);
}

void	exe_cmds_hd(int ac, char **av, char **env, int read_pipe)
{
	int	p_fds2[2];

	ft_pipe(p_fds2);				//read_pipe, p_fds2[0], p_fds2[1] 
//printf("piped(p_fds2), read_pipe = %d, p_fds2[0] = %d, p_fds2[1] = %d\n", read_pipe, p_fds2[0], p_fds2[1]);
	if (ft_fork() == 0)
	{
		ft_close(p_fds2[0]);			//p_fds[0], p_fds2[1] 
		exe_pipe_proc(*av, env, read_pipe, p_fds2[1]);
	}
	ft_close(p_fds2[1]);
	ft_close(read_pipe);			//p_fds2[0]
//printf("closed(p_fds2[1]), closed(read_pipe), p_fds2[0] = %d\n", p_fds2[0]);
	read_pipe = exe_mid_cmds(ac - 3, av + 1, env, p_fds2[0]);	//read_pipe
	exe_last_proc(av[ac - 2], av[ac - 1], env, read_pipe, APPEND_F);
}

void	exe_cmds_no_hd(int ac, char **av, char **env, int *p_fds)
{
	if (ft_fork() == 0)
	{
		ft_close(p_fds[0]);			//p_fsd[1]
		exe_first_proc(av[1], av[0], env, p_fds[1]);
	}
	ft_close(p_fds[1]);				//p_fds[0]
	p_fds[0] = exe_mid_cmds(ac - 4, av + 2, env, p_fds[0]);	//p_fds[0]
	exe_last_proc(av[ac - 2], av[ac - 1], env, p_fds[0], CREAT_F);
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
	pipe(p_fds);
	hd = is_str("here_doc", av[1]);
	if (hd)
		here_doc(p_fds[1], av[2]);
	av += 1 + hd * 2;
	ac -= 1 + hd * 2;
	if (hd)
		exe_cmds_hd(ac, av, env, p_fds[0]);
	else
		exe_cmds_no_hd(ac, av, env, p_fds);
}
