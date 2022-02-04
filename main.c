/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 21:25:15 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/04 21:25:30 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **av, char **env)
{
	int	p_fds[2];
	int	child_pid;

	if (ac != 5)
	{
		write(2, "pipex: Error: nb of arguments isn't 4\n", 38);
		return (1);
	}
	if (pipe(p_fds) == -1)
		exit_with_error(NULL, NULL, 1);
	child_pid = fork();
	if (child_pid == -1)
		exit_with_error(NULL, NULL, 1);
	if (child_pid == 0)
		exe_cmd2(av[3], av[4], env, p_fds);
	exe_cmd1(av[1], av[2], env, p_fds);
}

