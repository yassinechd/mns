#include "../include/minishell.h"

static int  *stdio_backup(void)
{
    static int save[2] = { -1, -1 };
    return (save);
}

int apply_redirs(t_command *c)
{
    t_redir *r;
    int fd;

    r = c->redirs;
    while (r)
    {
        if (r->type == R_IN)
            fd = open(r->file, O_RDONLY);
        else if (r->type == R_OUT)
            fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (r->type == R_APPEND)
            fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else /* heredoc est déjà géré avant */
        {
            r = r->next;
            continue;
        }
        if (fd < 0)
            return (perror(r->file), -1);
        if (r->type == R_IN && dup2(fd, STDIN_FILENO) < 0)
            return (close(fd), -1);
        if (r->type != R_IN && dup2(fd, STDOUT_FILENO) < 0)
            return (close(fd), -1);
        close(fd);
        r = r->next;
    }
    return (0);
}

void    restore_stdio(void)
{
    int *save = stdio_backup();

    if (save[0] != -1)
    {
        dup2(save[0], STDIN_FILENO);
        dup2(save[1], STDOUT_FILENO);
        close(save[0]);
        close(save[1]);
        save[0] = save[1] = -1;
    }
}

void    backup_stdio(void)
{
    int *save = stdio_backup();

    if (save[0] == -1)
    {
        save[0] = dup(STDIN_FILENO);
        save[1] = dup(STDOUT_FILENO);
    }
}

/******************************** PATH_UTILS *********************************/

/* Tente d'exécuter argv[0] dans chaque répertoire de paths[]. Si paths == NULL
   on se contente d'appeler execve avec argv[0] (déjà un chemin absolu ou relatif). */

void    exec_in_path(char **argv, char **paths, char **envp)
{
    int     i;
    char    *full;

    if (!paths)
        execve(argv[0], argv, envp);
    i = -1;
    while (paths[++i])
    {
        full = ft_strjoin(paths[i], argv[0]);
        if (!full)
            continue;
        execve(full, argv, envp);
        free(full);
    }
    /* si aucune n'a marché -> laisse le parent gérer le perror */
}

/******************************* BUILTIN_UTILS *******************************/

int is_parent_builtin(const char *name)
{
    return (!ft_strcmp(name, "cd") || !ft_strcmp(name, "export") ||
            !ft_strcmp(name, "unset") || !ft_strcmp(name, "exit"));
}

/********************************* HEREDOC **********************************/

/* Crée un pipe contenant le texte déjà stocké dans r->file (temp) et duplique
   l'extrémité lecture sur STDIN. On suppose que la partie collecte (<<) a été
   faite au parsing. */

int handle_heredoc(t_command *c)
{
    t_redir *r;
    int     pfd[2];
    int     fd;
    char    buf[4096];
    ssize_t rd;

    r = c->redirs;
    while (r)
    {
        if (r->type == R_HEREDOC)
        {
            if (pipe(pfd) < 0)
                return (-1);
            fd = open(r->file, O_RDONLY);
            if (fd < 0)
                return (perror(r->file), -1);
            while ((rd = read(fd, buf, sizeof(buf))) > 0)
                write(pfd[1], buf, rd);
            close(fd);
            close(pfd[1]);
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[0]);
        }
        r = r->next;
    }
    return (0);
}

/********************************* FD_UTILS *********************************/

void    close_all_pipes(int p[2][2])
{
    close(p[0][0]);
    close(p[0][1]);
    close(p[1][0]);
    close(p[1][1]);
}