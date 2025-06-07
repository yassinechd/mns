#include "../include/minishell.h"

int		g_exit_status = 0;


t_minishell	*ft_init_minishell(int ac, char **av, char **envp)
{
	t_minishell	*minishell;

	(void)ac;
	(void)av;
	minishell = malloc(sizeof(t_minishell));
	ft_strdup_tab(&minishell, envp);
	minishell->token = NULL;
	minishell->h_history = NULL;
	minishell->cmd = -1;
	return (minishell);
}


void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();         
		g_exit_status = 130;
	}
}

void	minishell_loop(t_minishell **minishell)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
		line = readline("minishell> ");
		if (!line)
			break;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		parse_token(tokens, minishell);
		free(line);
	}
	return ;
}

int main(int ac, char **av, char **envp)
{
    t_minishell *minishell;

    minishell = ft_init_minishell(ac, av, envp);
    minishell_loop(&minishell);
}