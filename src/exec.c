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

int ft_is_built(char **arg)
{
    if (ft_strcmp(arg[0], "echo") == 0)
        return (0);
    else if (ft_strcmp(arg[0], "cd") == 0)
        return (0);
    else if (ft_strcmp(arg[0], "pwd") == 0)
        return (0);
    else if (ft_strcmp(arg[0], "export") == 0)
        return (0);
    else if (ft_strcmp(arg[0], "unset") == 0)
        return (0);
    else if (ft_strcmp(arg[0], "env") == 0)
        return (0);
    else if (ft_strcmp(arg[0], "exit") == 0)
        return (0);
    return (1);
    
}

static int  exec_external(char **argv, char **paths, char **envp)
{
    int     i;
    char    *full;

    if (!argv || !argv[0])
        return (127);
    i = 0;
    while (paths && paths[i])
    {
        full = ft_strjoin(paths[i], argv[0]);
        execve(full, argv, envp);
        free(full);
        i++;
    }
    execve(argv[0], argv, envp);
    return (127);
}

static void fork_child(t_command *c, char **p, t_minishell **ms,
                       int pipe_fd[2])
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        if (pipe_fd[0] != -1)
        {
            dup2(pipe_fd[0], 0);
            close(pipe_fd[0]);
        }
        if (pipe_fd[1] != -1)
        {
            dup2(pipe_fd[1], 1);
            close(pipe_fd[1]);
        }
        if (ft_is_built(c->argv) == 0)
            exit(exec_builtin(c->argv, &(*ms)->envp));
        exit(exec_external(c->argv, p, (*ms)->envp));
    }
}

static void wait_children_set_status(t_minishell **ms)
{
    int status;

    status = 0;
    while (wait(&status) > 0)
        ;
    if (WIFEXITED(status))
        (*ms)->last_status = WEXITSTATUS(status);
    else
        (*ms)->last_status = 1;
}

int exec_single(t_command *cmd, t_minishell **ms, char **paths)
{
    int status;

    if (!cmd || !cmd->argv)
        return (1);
    if (ft_is_built(cmd->argv) == 0)
    {
        (*ms)->last_status = exec_builtin(cmd->argv, &(*ms)->envp);
        return ((*ms)->last_status);
    }
    fork_child(cmd, paths, ms, (int[2]){-1, -1});
    wait(&status);
    if (WIFEXITED(status))
        (*ms)->last_status = WEXITSTATUS(status);
    else
        (*ms)->last_status = 1;
    return ((*ms)->last_status);
}

int exec_pipeline(t_command *cmd, t_minishell **ms, char **paths, int n)
{
    int fd[2];
    int prev_in;
    int i;

    prev_in = -1;
    i = 0;
    while (i < n && cmd)
    {
        if (i < n - 1 && pipe(fd) == -1)
            return ((*ms)->last_status = 1);
        if (i == n - 1)
            fd[1] = -1;
        fork_child(cmd, paths, ms, (int[2]){prev_in, fd[1]});
        if (prev_in != -1)
            close(prev_in);
        if (fd[1] != -1)
            close(fd[1]);
        if (i < n - 1)
            prev_in = fd[0];
        else
            prev_in = -1;
        cmd = cmd->next;
        i++;
    }
    wait_children_set_status(ms);
    return ((*ms)->last_status);
}




void	handle_exec(t_command *cmd, t_minishell **ms)
{
	char	**paths;
	int		nb_cmds;

	paths = ft_find_path((*ms)->envp);
	if (!paths)
		return ;
	ft_add_slash(paths);

	nb_cmds = count_cmds(cmd);

	if (nb_cmds == 1)
		exec_single(cmd, ms, paths);
	else
		exec_pipeline(cmd, ms, paths, nb_cmds);

	ft_free_tab(paths);
}
