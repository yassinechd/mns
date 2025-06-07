#include "../include/minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void print_command_node(t_command *cmd)
{
	if (!cmd)
	{
		printf("Command is NULL.\n");
		return;
	}

	printf("---- Command Node ----\n");
	printf("Next Delimiter: %s\n", cmd->next_deli ? cmd->next_deli : "NULL");
	printf("Argc: %d\n", cmd->argc);
	printf("Size: %d\n", cmd->size);

	printf("Argv: ");
	if (cmd->argv)
	{
		for (int i = 0; i < cmd->argc; i++)
			printf("\"%s\" ", cmd->argv[i]);
		printf("\n");
	}
	else
	{
		printf("NULL\n");
	}

	printf("Infile: %s\n", cmd->infile ? cmd->infile : "NULL");
	printf("Outfile: %s\n", cmd->outfile ? cmd->outfile : "NULL");
	printf("Append: %d\n", cmd->append);
	printf("Heredoc: %d\n", cmd->heredoc);
	printf("Heredoc Delim: %s\n", cmd->heredoc_delim ? cmd->heredoc_delim : "NULL");
	printf("Pipe FDs: [%d, %d]\n", cmd->pipe_fd[0], cmd->pipe_fd[1]);

	printf("Next: %s\n", cmd->next ? "Exists" : "NULL");
	printf("----------------------\n");
}

int count_cmds(const t_command *cmd)
{
    int                count = 0;
    const t_command   *cur   = cmd;   /* ← copie locale du pointeur */

    while (cur)                       /* on parcourt jusqu’à NULL   */
    {
        ++count;
        cur = cur->next;              /* étape suivante             */
    }
    return count;
}
char	**ft_find_path(char **envp)
{
	char	**path;
	int		i;

	i = 0;
	path = NULL;
	
	while (envp[i])
	{	
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i], ':');
			if (!path)
				return (NULL);
		}
		i++;
	}
	path[0] = ft_substr(path[0], 5, ft_strlen(path[0]));
	return (path);
}

void	ft_add_slash(char **paths)
{
	int i;

	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		i++;
	}
}

void    handle_exec(t_command *cmd, t_minishell **ms, int cmd_count) //boucle sur cmd pr gerer l'exec
{
    char    **paths;


    paths = ft_find_path((*ms)->envp);
    ft_add_slash(paths);
    if (!paths)
        return ;
    (void)cmd_count;
    while (cmd)
    {
        /*
		(void)cmd_count;
		int cmd_count1 = count_cmds(cmd);
        
        //i = 0;
		if (ft_is_built(cmd->argv) == 0)
            exec_builtin(cmd->argv, &(*ms)->envp);
        else
            ft_dispatch_exec(cmd, ms, cmd_count1);
        //ft_dispatch_exec(cmd, ms, cmd_count1);*/
        print_command_node(cmd);   
        cmd = cmd->next;

    }
}