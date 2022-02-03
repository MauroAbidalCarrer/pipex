#include "header.h"

int	sub_cat(char *str, int sub_len, char *str2, char **dst);//demaner a jeremmy parraport au priorites
void	setup_cmd_av(char *cmd_s, t_cmd *cmd, char **env);
void	exe_cmd1(char *cmd_s, char *file, char **env, int p_fds[2]);
void	exe_cmd2(char *cmd_s, char *file, char **env, int p_fds[2]);

void	print_cmd(t_cmd cmd)
{
	printf("----------------\nprinting cmd\nto_free = %d\n", cmd.to_free);
	for (int i = 0; i < cmd.to_free; i++)
		printf("av[%d] = \"%s\"\n", i, cmd.av[i]);
	printf("cmd.path = \"%s\"\n-------------------\n", cmd.path);
}
//take care of ./: OUI!!!!!!!!!!!!!!!!!!!!!!!!
//does X_OK checks if the file exists?: OUI!!!
int	main(int ac, char **av, char **env)
{
/*
	int ret = access(av[1], R_OK);
	if (ret == -1)
		perror("yes");
	printf("ret = %d\n", ret);

	t_cmd cmd;
	cmd.not_found = 0;
	cmd.path = NULL;
	cmd.to_free = 1;
	cmd.av = malloc(sizeof(char *) * 2);
	if (sub_cat(av[1], index_of(0, av[1], 0), NULL, cmd.av))
	{
		printf("plus despacs????\n");
		return (1);
	}
	cmd.av[1] = NULL;
	if (!special_case(&cmd))
		get_cmd_path(&cmd, env);
	print_cmd(cmd);
	free(cmd.av);

	t_cmd cmd;
	cmd.not_found = 0;
	cmd.to_free = 0;
	cmd.path = NULL;
	setup_cmd_av(av[1], &cmd, env);
	print_cmd(cmd);
	int	p_fds[2];
	int	is_child;
	
	pipe(p_fds);
	is_child = fork();
	if (is_child)
		exe_cmd1(av[1], av[2], env, p_fds);
	dup2(p_fds[0], OUT);

	int fd = open(av[1], O_RDONLY);
	int s = 10;
	char str[s];
	dup2(fd, IN);
	printf("read %ld bytes\n", read(IN, str, s - 1));
	str[s - 1] = 0;
	printf("str = \"%s\"\n", str);
	close(fd);
[
	int	p_fds[2];
	int	is_child;
	
	pipe(p_fds);
	is_child = fork();
	if (is_child)
		exe_cmd1(av[1], av[2], env, p_fds);
	exe_cmd2(av[3], av[4], env, p_fds);
*/
}
