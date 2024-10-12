#define PIPEX_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../include/libft/libft.h"
#include "../include/ft_printf/src/ft_printf.h"

#define BINPATH "/bin/"
#define CATPATH "/bin/cat"
#define RDEND 0
#define WREND 1
#define IPIPE 0
#define OPIPE 1

typedef struct s_cmd
{
    char *cmd;
    char **argv;
    char **env;
    struct s_cmd *next;
} t_cmd;

typedef struct s_metad
{
    struct s_cmd **head;
    char *ifile;
    char *ofile;
} t_metad;

void ft_printcmdnode(t_cmd *cmd);
void ft_printcmdlist(t_cmd **head);
