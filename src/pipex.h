/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andoitzcp <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:14 by andoitzcp         #+#    #+#             */
/*   Updated: 2025/04/02 13:15:26 by andoitzcp        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define PIPEX_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "../include/libft/libft.h"
#include "../include/ft_printf/src/ft_printf.h"

#define ENVPATHSTR "PATH"
#define CATPATH "/bin/cat"
#define BINPATH
#define RDEND 0
#define WREND 1
#define IPIPE 0
#define OPIPE 1

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_metad
{
	struct s_cmd	**head;
	char			*ifile;
	char			*ofile;
	char			**env;
	char			*path;
}	t_metad;

void	ft_printcmdnode(t_cmd *cmd);
void	ft_printcmdlist(t_cmd **head);

/* utils */
char	*get_envvar(char *var, char **env);
char	*get_cmdpath(t_cmd *cmd, char **env);
void	ft_cleanup(t_metad *metad);
t_metad	ft_initmetad(int argc, char **argv, char **env);
int		handle_error(t_metad *md, char *s, int errornumber);

/* Handle redirs */
void	handle_left_redir(t_metad *md);
int		handle_right_redir(t_metad *md);

/* Parse */
t_cmd	*ft_buildcmdnode(char *scmd);
t_cmd	**ft_buildcmdlist(t_cmd **head, char **argv);
