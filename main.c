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
}

