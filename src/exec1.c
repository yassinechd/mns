#include "../include/minishell.h"


//--- gere pipe ---
//--- gere commande solo ---
//--- gere pas plusieur pipe ---
//--- redir > >> fonctionne pas ---
//--- redir input fonctionne	<  ---
//--- herdoc fonctionne			<< ---

int ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
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

void exec_single(char *path, t_command *cmd, char **envp) //execute 1 seul commande
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
        return ;
    if (pid == 0)
        execve(path, cmd->argv, envp);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        g_exit_status = 128 + WTERMSIG(status);
    

}
void	exec_cmd2(t_command *cmd, t_minishell **ms) //exec 1 seul commande
{
	char	**paths;
	char	*testpath;
	int		i;

	paths = ft_find_path((*ms)->envp);
    i = 0;
    /*while (paths[i])
    {
        printf("path[%d] : %s\n", i, paths[i]);
        i++;
    }*/
    ft_add_slash(paths);
    i = 0;
	if (!paths)
		return ;
    /*if (cmd->next_deli && ft_strncmp(cmd->next_deli, "|", 1) == 0)
        execute_pipe((*ms)->envp, cmd->argv, cmd->next_deli, testpath);*/
	if (ft_strchr(cmd->argv[0], '/') == 0 && access(cmd->argv[0], X_OK) == 0)
        exec_single(cmd->argv[0], cmd, (*ms)->envp);
	else
    {
        while (paths[i])
		{
            testpath = NULL;
		    testpath = ft_strjoin(paths[i], cmd->argv[0]);
		    if (access(testpath, X_OK) == 0)
		    {
				printf("-- ok -- %s\n", testpath);
				exec_single(testpath, cmd, (*ms)->envp);
    			free(testpath);
	    		break ;
    		}
			free(testpath);
    		i++;
		}
	}
    ft_free_tab(paths);
}



int	exec_pipe(t_command	**cmd, t_minishell	**ms) // gere les pipe, pas plusieur encore
{
	int			status;
	pid_t		p1;
	pid_t		p2;
	t_command	*left;
	t_command	*right;

	left = *cmd;
	right = (*cmd)->next;

	if (pipe((*cmd)->pipe_fd) == -1)
		return (perror("pipe"), -1);
	p1 = fork();
	if (p1 < 0)
        return (close((*cmd)->pipe_fd[0]), close((*cmd)->pipe_fd[1]), -1);
	if (p1 == 0)
	{
		dup2((*cmd)->pipe_fd[1], STDOUT_FILENO);
        close((*cmd)->pipe_fd[0]);
        close((*cmd)->pipe_fd[1]);
		/*if (left->infile || left->outfile || left->append || left->heredoc)
            setup_redir(left);*/
        exec_cmd2(left, ms);
        exit(EXIT_FAILURE);
	}
	p2 = fork();
	if (p2 < 0)
        return (close((*cmd)->pipe_fd[0]), close((*cmd)->pipe_fd[1]), -1);
	if (p2 == 0)
    {
        dup2((*cmd)->pipe_fd[0], STDIN_FILENO);
        close((*cmd)->pipe_fd[1]);
        close((*cmd)->pipe_fd[0]);
        /*if (right->infile || right->outfile || right->append || right->heredoc)
            setup_redir(right);*/
        exec_cmd2(right, ms);
        exit(EXIT_FAILURE);
    }
	close((*cmd)->pipe_fd[0]);
    close((*cmd)->pipe_fd[1]);
	waitpid(p1, &status, 0);
    printf("pppp\n");
    waitpid(p2, &status, 0);
    printf("pppssp\n");
	*cmd = (*cmd)->next;
	return (0);
}

// direeee
static int	open_heredoc(t_command *cmd)
{
	int		tmp_fd[2];
	char	*line;

	if (pipe(tmp_fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || !strncmp(line, cmd->heredoc_delim,
                              ft_strlen(cmd->heredoc_delim) + 1))
		write(tmp_fd[1], line, ft_strlen(line));
		write(tmp_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(tmp_fd[1]);
	return (tmp_fd[0]);		/* fd prêt à être dup2() sur STDIN */
}

int setup_redir(t_command *cmd) // tente de gerer les redir
{
    int fd_in = -1;
    int fd_out = -1;

    if (cmd->infile)
        if ((fd_in = open(cmd->infile, O_RDONLY)) < 0)
            return (perror(cmd->infile), -1);
    if (cmd->heredoc)
        if ((fd_in = open_heredoc(cmd)) < 0)
            return (perror("heredoc pipe"), -1);
    if (cmd->outfile || cmd->append)
    {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        if ((fd_out = open(cmd->outfile, flags, 0644)) < 0)
            return (perror(cmd->outfile), -1);
    }
    if (fd_in  != -1 && dup2(fd_in,  STDIN_FILENO)  == -1)
        perror("dup2(in)");
    if (fd_out != -1 && dup2(fd_out, STDOUT_FILENO) == -1)
        perror("dup2(out)");
    if (fd_in  != -1) close(fd_in);
    if (fd_out != -1) close(fd_out);
    return (0);
}
/* -------------------------------------------------------------
 *  in_fd :  fd à connecter sur STDIN de la commande courante
 *           (STDIN_FILENO pour la première)
 *  cmd   :  maillon courant de la liste t_command
 *  ms    :  pointeur sur votre structure centrale
 * ------------------------------------------------------------- */
static int  exec_pipeline_rec(int in_fd, t_command *cmd, t_minishell **ms)
{
    int   pfd[2];
    pid_t pid;

    /* ---------- DERNIÈRE COMMANDE : pas de pipe de sortie ---------- */
    if (cmd->next == NULL)
    {
        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (cmd->infile || cmd->outfile || cmd->append || cmd->heredoc)
            setup_redir(cmd);
        exec_cmd2(cmd, ms);
        perror("exec_cmd2");          /* atteint seulement si exec échoue */
        _exit(EXIT_FAILURE);
    }

    /* ---------- PIPE SUIVANT ---------- */
    if (pipe(pfd) == -1)
        return (close(in_fd), perror("pipe"), -1);

    pid = fork();
    if (pid < 0)
        return (close(in_fd), close(pfd[0]), close(pfd[1]), -1);

    if (pid == 0)                                  /* ----- Fils ----- */
    {
        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[0]);                             /* inutile dans le fils */
        close(pfd[1]);

        if (cmd->infile || cmd->outfile || cmd->append || cmd->heredoc)
            setup_redir(cmd);
        exec_cmd2(cmd, ms);
        perror("exec_cmd2");
        _exit(EXIT_FAILURE);
    }

    /* ---------- Père ---------- */
    close(in_fd);          /* l’ancien read-end ne sert plus au père      */
    close(pfd[1]);         /* seul le fils gauche écrit dans ce pipe      */

    /* Appel récursif pour le reste du pipeline.
       S’il remonte une erreur, on s’arrête. */
    if (exec_pipeline_rec(pfd[0], cmd->next, ms) == -1)
        return -1;

    /* On attend le fils gauche une fois que tout le pipeline est lancé */
    waitpid(pid, NULL, 0);
    return 0;
}

/* ------------------------------------------------------------------ */
/* Point d’entrée : à appeler depuis votre gestionnaire d’exec.        */
int exec_pipeline(t_command *cmd, t_minishell **ms)
{
    return exec_pipeline_rec(STDIN_FILENO, cmd, ms);
}



int	ft_dispatch_exec(t_command *cmd, t_minishell **ms, int cmd_count) // dispatch si il faux exec un pipe, une redir ou une commande simple
{
	if (cmd_count == 1)
    	exec_cmd2(cmd, ms);
	else
    	exec_pipe(&cmd, ms);

	/*(void)cmd_count;
	if (!cmd)
		return (0);
	if (cmd_count == 1)
		return (exec_cmd2(cmd, ms), 0);
	if (cmd->next_deli && ft_strcmp(cmd->next_deli, "|") == 0)
		return (exec_pipe(&cmd, ms));
	if (cmd->infile || cmd->outfile || cmd->append || cmd->heredoc)
        return (setup_redir(cmd));*/
	return (0);
}

/* Compte le nombre de commandes (maillons) dans la liste t_command.
 * ──────────────────────────────────────────────────────────────────
 *  cmd  : pointeur vers la tête de la liste (peut être NULL)
 *  ret  : 0 si la liste est vide, sinon le nombre de maillons
 *  NB   : la fonction ne modifie jamais *cmd.
 */
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

#include <stdio.h>

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


void    handle_exec(t_command *cmd, t_minishell **ms, int cmd_count) //boucle sur cmd pr gerer l'exec
{
    char    **paths;
    //char    *tespath;
    //int i;

    paths = ft_find_path((*ms)->envp);
    ft_add_slash(paths);
    if (!paths)
        return ;
    while (cmd)
    {
		/*(void)cmd_count;
		int cmd_count1 = count_cmds(cmd);
        print_command_node(cmd);
        //i = 0;
        //tespath = NULL;
		if (ft_is_built(cmd->argv) == 0)
        {
            

            printf("\nis built\n");
            exec_builtin(cmd->argv, &(*ms)->envp);
        }
            
        else
        {
            printf("\nis not built\n");
            ft_dispatch_exec(cmd, ms, cmd_count1);
        }
        //ft_dispatch_exec(cmd, ms, cmd_count1);*/
            
        cmd = cmd->next;

    }
}