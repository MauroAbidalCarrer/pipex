/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 21:25:15 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/05 17:16:54 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **av, char **env)
{
	int	p_fds[2];

	if (ac != 5)
	{
		write(2, "pipex: Error: nb of arguments isn't 4\n", 38);
		return (1);
	}
	ft_pipe(p_fds);
	if (ft_fork() == 0)
	{
		ft_close(p_fds[0]);
		exe_first(av[2], av[1], env, p_fds[1]);
	}
	exe_last(av[3], av[4], env, p_fds);
}