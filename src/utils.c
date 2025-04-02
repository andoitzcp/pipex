#include "pipex.h"


int handle_error(t_metad *md, char *s, int errornumber)
{
    perror(s);
    //ft_printf("%s:%s", s, strerror(errornumber));
    if (md)
        ft_cleanup(md);
    exit(errornumber);
    //exit(EXIT_FAILURE);
}

char *get_envvar(char *var, char **env)
{
	 char *s;
	 int i;

	 i = 0;
	 while (env[i] != NULL)
	 {
		  if (ft_strncmp(var, env[i], ft_strlen(var)) == 0)
		  {
			  s = ft_strchr(env[i], '=') + 1;
			  return (ft_strdup(s));
		  }
		  i++;
	 }
	 return(ft_strdup(""));
}

char *get_cmdpath(t_cmd *cmd, char **env)
{
	 char *s;
	 char **paths;
	 char *path;
	 int i;

	 s = get_envvar("PATH", env);
	 paths = ft_split(s, ':');
	 free(s);
	 i = 0;
	 while (paths[i] != NULL)
	 {
		  s = ft_strjoin(paths[i], "/");
		  path = ft_strjoin(s, cmd->cmd);
		  free(s);
		  if (access(path, F_OK | X_OK) == 0)
		  {
			   ft_free2parray(paths);
			   return (path);
		  }
		  free(path);
		  i++;
	 }
	 ft_free2parray(paths);
	 return(cmd->cmd);
}

void ft_cleanup(t_metad *metad)
{
    t_cmd *cmd;
    t_cmd *tmp;

    cmd = *(metad->head);
    while (cmd)
    {
        ft_free2parray(cmd->argv);
        tmp = cmd;
        cmd = cmd->next;
        free(tmp);
    }
    free(metad->head);
}

t_metad ft_initmetad(int argc, char **argv, char **env)
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
    metad.env = env;
    return (metad);
}
