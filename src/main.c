#include "pipex.h"
#include <fcntl.h>

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
        exit(EXIT_FAILURE);
    cmd->argv = ft_split(scmd, ' ');
    dstlen = ft_strlen(cmd->argv[0]) + ft_strlen(BINPATH) + 1;
    cmd->cmd = ft_calloc(dstlen, sizeof(char));
    ft_strlcat(cmd->cmd, BINPATH, dstlen);
    ft_strlcat(cmd->cmd, cmd->argv[0], dstlen);
    cmd->env = NULL;
    return (cmd);
}

t_cmd *ft_buildfirstnode(char *infile, char *cmd1)
{
    t_cmd *cmd;
    char **new_argv;
    int i;

    cmd = ft_parsecmd(cmd1);
    i = 0;
    while(cmd->argv[i])
        i++;
    new_argv = ft_calloc(i + 2, sizeof(char **));
    i = 0;
    while(cmd->argv[i])
    {
        new_argv[i] = cmd->argv[i];
        i++;
    }
    new_argv[i] = infile;
    i = 0;
    cmd->argv = new_argv;
    return (cmd);
}

t_cmd **ft_buildcmdlist(t_cmd **head, int argc, char **argv)
{
    t_cmd *node;
    int i;

    node = ft_buildfirstnode(argv[1], argv[2]);
    *head = node;
    i = 2;
    while (++i < argc)
    {
        node->next = ft_parsecmd(argv[i]);
        node = node->next;
    }
    node->next = NULL;
    return (head);
}

void ft_cleanup(t_cmd *cmd)
{
    printf("Esto no deberia imprimirse: %s\n", cmd->cmd);
}

void ft_execcmd(t_cmd *cmd)
{
    static int fd[2][2] = {{-1, -1}, {-1, -1}};
    int fd_fileout;
    pid_t pid;

    if (cmd->next)
    {
        if (pipe(fd[OPIPE]) == -1)
            ft_cleanup(cmd);
    }
    if ((pid = fork()) == -1)
        ft_cleanup(cmd);
    if (pid == 0)
    {
        if (fd[IPIPE][RDEND] != -1)
        {
            dup2(fd[IPIPE][RDEND], STDIN_FILENO);
            close(fd[IPIPE][RDEND]);
        }
        if (cmd->next)
            dup2(fd[OPIPE][WREND], STDOUT_FILENO);
        else
        {
            fd_fileout = open("outfile", O_CREAT | O_WRONLY);
            dup2(fd[OPIPE][WREND], fd_fileout);
        }
        close(fd[OPIPE][RDEND]);
        execve(cmd->cmd, cmd->argv, cmd->env);
        close(fd[OPIPE][WREND]);
    }
    fd[IPIPE][RDEND] = fd[OPIPE][RDEND];
    fd[IPIPE][WREND] = fd[OPIPE][WREND];
    close(fd[IPIPE][WREND]);
    waitpid(pid, NULL, 0);
}

int main(int argc, char **argv)
{
    t_cmd *head;

    //ft_checkargs(argc, argv);
    //ft_checkiofiles(argv[1], argv[4]);
    ft_buildcmdlist(&head, argc, argv);
    //ft_printcmdlist(&head);
    while (head)
    {
        ft_execcmd(head);
        head = head->next;
    }
    //head = head->next;
    //ft_execcmd(head->next);
    //while (head)
    //{
        //ft_execcmd(head);
        ////printf("node: %p\tcmd: %s\targv[0]:%s\n", head, head->cmd, head->argv[0]);
        //head = head->next;
    //}
    return (0);

}
