#ifndef	MINISHELL_H
#define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>

# include "readline/readline.h"
# include "readline/history.h"



extern int g_exit_status;

typedef struct	s_cmd
{
    char			*value;
    struct s_cmd	*next;
}				t_cmd;

typedef struct	s_minishell
{
	int				cmd;
    char			**envp;
    t_list			*token;
	t_list			*h_history;

}				t_minishell;

typedef struct  s_token
{
    char				*value;
    int					type;
    struct s_token		*next;
}               t_token;


typedef struct s_command
{
	char				*next_deli;
	char				**argv;
	int					argc;
	int					size;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	char				*heredoc_delim;
	int					pipe_fd[2];
	struct s_command	*next;
}	t_command;

//utils
int		ft_strlen_tab(char **tab);
void	ft_strdup_tab(t_minishell **minishell, char **tab);
void	ft_free_tab(char **tab);

//lexer
t_token	*lexer(char *prompt);

//parser
void parse_token(t_token *toklist, t_minishell **minishell);

//tok-split
char	**ft_split_tokens(char *s);
char	**ft_free_split(char **tab);

//tok-list
t_token	*build_list(char **tab);
int	get_type(const char *tok);

//exec
void    handle_exec(t_command *cmd, t_minishell **ms, int cmd_count);
int ft_strcmp(char *s1, char *s2);

// built-in
int exec_builtin(char **args, char ***env);
int ft_echo(char **args);
int ft_cd(char **args, char ***env);
int ft_pwd(char **args);
int	ft_exit(char **args);
int add_or_update_env(char *key, char *value, char ***env);
int cmp_env(const void *a, const void *b);
int ft_export(char **args, char ***env);
int remove_env(char *key, char ***env);
int ft_unset(char **args, char ***env);
int ft_env(char **args, char **env);
# endif