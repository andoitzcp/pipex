#include "pipex.h"
#include <fcntl.h>
#include <stdlib.h>
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

t_cmd *ft_parsecmd(char *scmd)
{
    t_cmd *cmd;
    int dstlen;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        perror("Malloc allocation:");
        exit(EXIT_FAILURE);
    }
    cmd->argv = ft_split(scmd, ' ');
    dstlen = ft_strlen(cmd->argv[0]) + ft_strlen(BINPATH) + 1;
    cmd->cmd = ft_calloc(dstlen, sizeof(char));
    if (!cmd->cmd)
    {
        perror("Malloc allocation:");
        exit(EXIT_FAILURE);
    }
    ft_strlcat(cmd->cmd, BINPATH, dstlen);
    ft_strlcat(cmd->cmd, cmd->argv[0], dstlen);
    cmd->env = NULL;
    return (cmd);
}

t_cmd *ft_buildfirstnode(char *infile)
{
    char *s;
    int dstlen;
    t_cmd *cmd;

    dstlen = ft_strlen("cat ") + ft_strlen(infile) + 1;
    s = ft_calloc(dstlen, sizeof(char));
    if (!s)
    {
        perror("Malloc allocation:");
        exit(EXIT_FAILURE);
    }
    ft_strlcat(s, "cat ", dstlen);
    ft_strlcat(s, infile, dstlen);
    cmd = ft_parsecmd(s);
    free(s);
    return (cmd);
}

t_cmd **ft_buildcmdlist(t_cmd **head, int argc, char **argv)
{
    t_cmd *node;
    int i;

    node = ft_buildfirstnode(argv[1]);
    *head = node;
    i = 1;
    while (++i < argc - 1)
    {
        node->next = ft_parsecmd(argv[i]);
        node = node->next;
    }
    node->next = NULL;
    return (head);
}

void ft_cleanup(t_metad *metad)
{
    t_cmd *cmd;
    t_cmd *tmp;

    cmd = *(metad->head);
    while (cmd)
    {
        free(cmd->cmd);
        ft_free2parray(cmd->argv);
        tmp = cmd;
        cmd = cmd->next;
        free(tmp);
    }
    free(metad->head);

}

void ft_childproc(t_cmd *cmd, int fd[2][2], char *ofile)
{
    int fd_fileout;

    if (fd[IPIPE][RDEND] != -1)
    {
        dup2(fd[IPIPE][RDEND], STDIN_FILENO);
        close(fd[IPIPE][RDEND]);
    }
    if (cmd->next)
        dup2(fd[OPIPE][WREND], STDOUT_FILENO);
    else
    {
        fd_fileout = open(ofile, O_CREAT | O_WRONLY, 0644);
        dup2(fd_fileout, STDOUT_FILENO);
        close(fd_fileout);
    }
    close(fd[OPIPE][RDEND]);
    execve(cmd->cmd, cmd->argv, cmd->env);
    close(fd[OPIPE][WREND]);
    if (!cmd->next)
        close(fd_fileout);
}

void ft_execcmd(t_cmd *cmd, t_metad *metad)
{
    static int fd[2][2] = {{-1, -1}, {-1, -1}};
    pid_t pid;

    if (cmd->next)
    {
        if (pipe(fd[OPIPE]) == -1)
            ft_cleanup(metad);
    }
    if ((pid = fork()) == -1)
        ft_cleanup(metad);
    if (pid == 0)
        ft_childproc(cmd, fd, metad->ofile);
    fd[IPIPE][RDEND] = fd[OPIPE][RDEND];
    fd[IPIPE][WREND] = fd[OPIPE][WREND];
    close(fd[IPIPE][WREND]);
    waitpid(pid, NULL, 0);
}

t_metad ft_initmetad(int argc, char **argv)
{
    t_metad metad;

    metad.head = malloc(sizeof(t_cmd **));
    if (!metad.head)
    {
        perror("Malloc allocation:");
        exit(EXIT_FAILURE);
    }
    metad.ifile = argv[1];
    metad.ofile = argv[argc - 1];
    return (metad);
}

int main(int argc, char **argv)
{
    t_metad metad;
    t_cmd *cmd;

    //ft_checkargs(argc, argv);
    //ft_checkiofiles(argv[1], argv[4]);
    metad = ft_initmetad(argc, argv);
    metad.head = ft_buildcmdlist(metad.head, argc, argv);
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
