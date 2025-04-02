/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleredirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andoitzcp <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:58:54 by andoitzcp         #+#    #+#             */
/*   Updated: 2025/04/02 13:27:56 by andoitzcp        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_left_redir(t_metad *md)
{
	int	fd_filein;

	fd_filein = open(md->ifile, O_RDONLY, 0777);
	if (fd_filein == -1)
		handle_error(md, "ft_childproc", errno);
	dup2(fd_filein, STDIN_FILENO);
	return ;
}

int	handle_right_redir(t_metad *md)
{
	int	fd_fileout;

	fd_fileout = open(md->ofile, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd_fileout == -1)
		handle_error(md, "ft_childproc", errno);
	dup2(fd_fileout, STDOUT_FILENO);
	close(fd_fileout);
	return (fd_fileout);
}
