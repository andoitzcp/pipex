/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andoitzcp <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:33:53 by andoitzcp         #+#    #+#             */
/*   Updated: 2025/04/02 13:14:10 by andoitzcp        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_checkargs(int argc, char **argv)
{
	int	i;

	if (argc != 5)
		handle_error(NULL, "ft_checkargs", EXIT_FAILURE);
	i = 0;
	while (i++ < argc - 1)
	{
		if (*argv[i] == '\0')
			handle_error(NULL, "ft_checkargs", EXIT_FAILURE);
	}
	return ;
}

void	ft_childproc(t_cmd *cmd, int fd[2][2], t_metad *md)
{
	int	fd_fileout;

	if (fd[IPIPE][RDEND] == -1)
		handle_left_redir(md);
	else if (fd[IPIPE][RDEND] != -1)
	{
		dup2(fd[IPIPE][RDEND], STDIN_FILENO);
		close(fd[IPIPE][RDEND]);
	}
	if (cmd->next)
		dup2(fd[OPIPE][WREND], STDOUT_FILENO);
	else
		fd_fileout = handle_right_redir(md);
	close(fd[OPIPE][RDEND]);
	if (execve(get_cmdpath(cmd, md->env), cmd->argv, md->env) == -1)
		handle_error(md, "ft_childproc", errno);
	close(fd[OPIPE][WREND]);
	if (!cmd->next)
		close(fd_fileout);
}

void	ft_execcmd(t_cmd *cmd, t_metad *md)
{
	static int	fd[2][2] = {{-1, -1}, {-1, -1}};
	pid_t		pid;

	if (cmd->next)
	{
		if (pipe(fd[OPIPE]) == -1)
			handle_error(md, "ft_execcmd", errno);
	}
	pid = fork();
	if (pid == -1)
		handle_error(md, "ft_execcmd", errno);
	if (pid == 0)
		ft_childproc(cmd, fd, md);
	fd[IPIPE][RDEND] = fd[OPIPE][RDEND];
	fd[IPIPE][WREND] = fd[OPIPE][WREND];
	close(fd[IPIPE][WREND]);
	waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **env)
{
	t_metad	metad;
	t_cmd	*cmd;

	ft_checkargs(argc, argv);
	metad = ft_initmetad(argc, argv, env);
	metad.head = ft_buildcmdlist(metad.head, argv);
	cmd = *(metad.head);
	while (cmd)
	{
		ft_execcmd(cmd, &metad);
		cmd = cmd->next;
	}
	ft_cleanup(&metad);
	return (0);
}
