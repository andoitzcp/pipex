#include "pipex.h"
#include <fcntl.h>
#include <unistd.h>



void ft_checkargs(int argc, char **argv)
{
	int i;
	if (argc != 5)
    {
        perror("Not a valid number of arguments\n");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (i++ < argc - 1)
    {
        if (*argv[i] == '\0')
        {
            perror("Argument %d must not be null");
            exit(EXIT_FAILURE);
        }
    }
    return ;
}

void ft_checkiofiles(char *ifile, char *ofile)
{
	if (access(ifile, R_OK) == -1)
    {
        perror("Error during io file checking: ");
        exit(EXIT_FAILURE);
    }
    if (access(ofile, W_OK) == -1)
    {
        perror("Error during io file checking: ");
        exit(EXIT_FAILURE);
    }
}

t_cmd *ft_buildcmdnode(char *scmd)
{
    t_cmd *cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        perror("Malloc allocation:");
        exit(EXIT_FAILURE);
    }
    cmd->argv = ft_split(scmd, ' ');
    cmd->cmd = *(cmd->argv);
	cmd->next = NULL;
    return (cmd);
}

t_cmd **ft_buildcmdlist(t_cmd **head, char **argv)
{
    t_cmd *node;

    node = ft_buildcmdnode(argv[2]);
    *head = node;
	node->next = ft_buildcmdnode(argv[3]);
	node = node->next;
    return (head);
}


void ft_childproc(t_cmd *cmd, int fd[2][2], t_metad *md)
{
    int fd_fileout;
    int fd_filein;

	printf("flag000: %s\n", cmd->cmd);
	printf("flag00: %d\n", fd[OPIPE][RDEND]);
	printf("flag01: %d\n", fd[OPIPE][WREND]);
	if (fd[IPIPE][RDEND] == -1)
	{
		printf("flag001: %s\n", cmd->cmd);
        fd_filein = open(md->ifile, O_RDONLY, 0777);
		dup2(fd_filein, STDIN_FILENO);
		printf("flag02: %d\n", fd[IPIPE][RDEND]);
		printf("flag03: %d\n", fd[IPIPE][WREND]);
        /* close(fd_filein); */
	}
	else if (fd[IPIPE][RDEND] != -1)
    {
		printf("flag002: %s\n", cmd->cmd);
        dup2(fd[IPIPE][RDEND], STDIN_FILENO);
        close(fd[IPIPE][RDEND]);
    }
    if (cmd->next)
	{
		printf("flag003: %s\n", cmd->cmd);
        dup2(fd[OPIPE][WREND], STDOUT_FILENO);
	}
    else
    {
		printf("flag004: %s\n", cmd->cmd);
        fd_fileout = open(md->ofile, O_CREAT | O_WRONLY | O_TRUNC, 0777);
        dup2(fd_fileout, STDOUT_FILENO);
        close(fd_fileout);
    }
    close(fd[OPIPE][RDEND]);
    execve(cmd->cmd, cmd->argv, md->env);
    close(fd[OPIPE][WREND]);
    if (!cmd->next)
        close(fd_fileout);
}

void ft_execcmd(t_cmd *cmd, t_metad *md)
{
    static int fd[2][2] = {{-1, -1}, {-1, -1}};
    pid_t pid;

    if (cmd->next)
    {
        if (pipe(fd[OPIPE]) == -1)
            ft_cleanup(md);
    }
    if ((pid = fork()) == -1)
        ft_cleanup(md);
    if (pid == 0)
        ft_childproc(cmd, fd, md);
    fd[IPIPE][RDEND] = fd[OPIPE][RDEND];
    fd[IPIPE][WREND] = fd[OPIPE][WREND];
    close(fd[IPIPE][WREND]);
    waitpid(pid, NULL, 0);
}


int main(int argc, char **argv, char **env)
{
    t_metad metad;
    t_cmd *cmd;

    ft_checkargs(argc, argv);
    ft_checkiofiles(argv[1], argv[4]);
    metad = ft_initmetad(argc, argv, env);
    metad.head = ft_buildcmdlist(metad.head, argv);
    //ft_printcmdlist(metad.head);
    cmd = *(metad.head);
    while (cmd)
    {
        ft_execcmd(cmd, &metad);
        cmd = cmd->next;
    }
    ft_cleanup(&metad);
    return (0);
}

/* int main (int argc, char **argv, char **env) */
/* { */
/* 	 int i; */
/*      t_cmd c; */

/*      printf("argc: %d, argv: %p\n", argc, argv); */
/* 	 i = 0; */
/* 	 while (env[i] != NULL) */
/* 	 { */
/* 		  printf("line: %d -> %s\n", i, env[i]); */
/* 		  i++; */
/* 	 } */
/* 	 printf("path var: %s\n", get_envvar("PATH", env)); */

/*      c.cmd = ft_strdup("cat"); */
/* 	 printf("path cmd: %s\n", get_cmdpath(&c, env)); */
/*      c.cmd = ft_strdup("emacs"); */
/* 	 printf("path cmd: %s\n", get_cmdpath(&c, env)); */
/*      c.cmd = ft_strdup("./leire"); */
/* 	 printf("path cmd: %s\n", get_cmdpath(&c, env)); */
/* 	 i = open("Makefile", O_RDONLY, 0777); */
/* 	 dup2(i, STDOUT_FILENO); */
/* 	 return (0); */
/* } */
