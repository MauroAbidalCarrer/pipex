/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 21:25:15 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/11 12:37:07 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	exe_mid_cmds(int nb_cmds, char **av, char **env, int c_read_p)
{
	int	p_fds2[2];

	while (nb_cmds--)
	{
		ft_pipe(p_fds2);
		if (ft_fork() == 0)
		{
			ft_close(p_fds2[0]);
			exe_pipe(*av, env, c_read_p, p_fds2[1]);
		}
		ft_close(p_fds2[1]);
		ft_close(c_read_p);
		c_read_p = p_fds2[0];
		av++;
	}
	return (c_read_p);
}

void	exe_cmds_no_hd(int ac, char **av, char **env, int *p_fds)
{
	int	p_read_n_open_f[2];

	if (ft_fork() == 0)
	{
		ft_close(p_fds[0]);
		exe_first(av[1], av[0], env, p_fds[1]);
	}
	ft_close(p_fds[1]);
	p_fds[0] = exe_mid_cmds(ac - 4, av + 2, env, p_fds[0]);
	p_read_n_open_f[0] = p_fds[0];
	p_read_n_open_f[1] = CREAT_F;
	exe_last(av[ac - 2], av[ac - 1], env, p_read_n_open_f);
}

int	main(int ac, char **av, char **env)
{
	int	p_fds[2];

	if (ac != 5)
	{
		write(2, "nb of args incorrect, write at 4 args\n", 43);
		return (1);
	}
	pipe(p_fds);
	exe_cmds_no_hd(ac - 1, av + 1, env, p_fds);
}
