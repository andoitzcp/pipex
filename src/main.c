#include "pipex.h"
#include <stdlib.h>

void ft_checkarguments(int argc, char **argv)
{
    int i;
    if (argc != 5)
    {
        perror("Not a valid number of arguments\n");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (i++ < argc)
    {
        if (*argv[i] == '\0')
        {
            perror("Argument %d must not be null");
            exit(EXIT_FAILURE);
        }
    }
    return ;
}

int main(int argc, char **argv)
{
    ft_checkarguments(argc, argv);

}
