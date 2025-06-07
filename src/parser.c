#include "../include/minishell.h"

t_command	*init_command(const char *word)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->size = 2;
	cmd->argc = 1;
	cmd->argv = malloc(sizeof(char *) * cmd->size);
	if (!cmd->argv)
		return (free(cmd), NULL);
	cmd->argv[0] = ft_strdup(word);
	if (!cmd->argv[0])
		return (free(cmd->argv), free(cmd), NULL);
    cmd->next_deli = NULL;
	cmd->argv[1] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}


int	handle_redir(t_command *cmd, t_token **cur)
{
	if (!(*cur)->next)
		return (-1);                           /* syntax error */
	if ((*cur)->type == 3)                     /* <  */
		cmd->infile = ft_strdup((*cur)->next->value);
	else if ((*cur)->type == 4)                /* >  */
		cmd->outfile = ft_strdup((*cur)->next->value);
	else if ((*cur)->type == 1)                /* >> */
	{
		cmd->outfile = ft_strdup((*cur)->next->value);
		cmd->append = 1;
	}
	else if ((*cur)->type == 2)                /* << */
	{
		cmd->heredoc = 1;
		cmd->heredoc_delim = ft_strdup((*cur)->next->value);
	}
	*cur = (*cur)->next;                       /* skip file/delim */
	return (0);
}

int	add_arg(t_command *cmd, const char *val)
{
	char	**n_av;
	int		n_size;

	if (cmd->argc + 1 >= cmd->size)
	{
		n_size = cmd->size * 2;
		n_av = malloc(sizeof(char *) * n_size);
		if (!n_av)
			return (-1);
		ft_memcpy(n_av, cmd->argv, sizeof(char *) * cmd->argc);
		free(cmd->argv);
		cmd->argv = n_av;
		cmd->size = n_size;
	}
	cmd->argv[cmd->argc] = ft_strdup(val);
	if (!cmd->argv[cmd->argc])
		return (-1);
	cmd->argc++;
	cmd->argv[cmd->argc] = NULL;
	return (0);
}

static int	add_word(t_command **cmd, t_token *cur, t_command **head)
{
	if (!*cmd)
	{
		*cmd = init_command(cur->value);
		if (!*cmd)
			return (-1);
		if (!*head)
			*head = *cmd;
	}
	else if (add_arg(*cmd, cur->value) < 0)
		return (-1);
	return (0);
}

void	ft_redirpip(t_command **cmd, t_token **cur, int type)
{

	if (!cmd || !*cmd || !cur || !*cur || !(*cur)->next)
		return ;

	if (type == 1)
	{
		(*cmd)->append = 1;
		if ((*cmd)->outfile)
			free((*cmd)->outfile);
		(*cmd)->outfile = ft_strdup((*cur)->next->value);
	}
	else if (type == 4)
	{
		(*cmd)->append = 0;
		if ((*cmd)->outfile)
			free((*cmd)->outfile);
		(*cmd)->outfile = ft_strdup((*cur)->next->value);
	}
	else if (type == 3)
	{
		if ((*cmd)->infile)
			free((*cmd)->infile);
		(*cmd)->infile = ft_strdup((*cur)->next->value);
	}
	else if (type == 2)
	{
		(*cmd)->heredoc = 1;
		if ((*cmd)->heredoc_delim)
			free((*cmd)->heredoc_delim);
		(*cmd)->heredoc_delim = ft_strdup((*cur)->next->value);
	}
	*cur = (*cur)->next;
}

/*static int	new_pipe(t_command **cmd, t_token **cur, int type)
{
	if (!(*cur)->next || !*cmd)
		return (-1);
    if (type >= 0 && type <= 4)
        ft_redirpip(cmd, cur, type);
	(*cmd)->next = init_command((*cur)->next->value);
	if (!(*cmd)->next)
		return (-1);
	*cmd = (*cmd)->next;
	*cur = (*cur)->next;
	return (0);
}*/

int guess_delim(t_command **cmd, t_token **cur, int type)
{
    if (!cmd || !*cmd)
        return (-1);
    if (type == 0)
        (*cmd)->next_deli = ft_strdup("|");
    else if (type == 1)
        (*cmd)->next_deli = ft_strdup(">>");
    else if (type == 2)
        (*cmd)->next_deli = ft_strdup("<<");
    else if (type == 3)
        (*cmd)->next_deli = ft_strdup("<");
    else if (type == 4)
        (*cmd)->next_deli = ft_strdup(">");
    (*cmd)->next = init_command((*cur)->next->value);
	if (!(*cmd)->next)
		return (-1);
	*cmd = (*cmd)->next;
	*cur = (*cur)->next;
	return (0);
}

static int	dispatch(t_command **cmd, t_token **cur, t_command **head)
{
	if ((*cur)->type == 5)
		return (add_word(cmd, *cur, head));
	else if ((*cur)->type >= 0 && (*cur)->type <= 4)
		return (guess_delim(cmd, cur, (*cur)->type));
	//else if ((*cur)->type >= 1 && (*cur)->type <= 4)
	//	return (handle_redir(*cmd, cur));
	return (0);
}



t_command	*ft_create_struct_command(t_token *tok)
{
	t_token		*cur;
	t_command	*cmd;
	t_command	*head;

	cur = tok;
	cmd = NULL;
	head = NULL;
	while (cur)
	{
		if (dispatch(&cmd, &cur, &head) < 0)
			return (NULL);
		cur = cur->next;
	}
	if (cmd)
		cmd->argv[cmd->argc] = NULL;
	return (head);
}

void ft_redir_setup(t_command *cmd)
{
    while (cmd)
    {
        if (get_type(cmd->next_deli) == 1)
        {
            cmd->append = 1;
            if (cmd->outfile)
                free(cmd->outfile);
            cmd->outfile = ft_strdup(cmd->next->argv[0]);
        }
        else if (get_type(cmd->next_deli) == 2)
        {
            cmd->heredoc = 1;
            if (cmd->heredoc_delim)
                free(cmd->heredoc_delim);
            cmd->heredoc_delim = ft_strdup(cmd->next->argv[0]);
        }
        else if (get_type(cmd->next_deli) == 3)
        {
            if (cmd->infile)
                free(cmd->infile);
            cmd->infile = ft_strdup(cmd->next->argv[0]);
        }
        else if (get_type(cmd->next_deli) == 4)
        {
            cmd->append = 0;
            if (cmd->outfile)
                free(cmd->outfile);
            cmd->outfile = ft_strdup(cmd->next->argv[0]);
        }
        cmd = cmd->next;
    }

}

int	ft_count_cmd(t_command *cmd)
{
	t_command	*head;
	int			i;

	head = cmd;
	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

void parse_token(t_token *toklist, t_minishell **minishell)
{

	t_command *cmd;

	cmd = ft_create_struct_command(toklist);
    ft_redir_setup(cmd);
	(*minishell)->cmd = ft_count_cmd(cmd);
    if (!cmd)
        printf("no cmd\n");
	
    handle_exec(cmd, minishell, (*minishell)->cmd);
	/*printf("\n=== Liste des tokens ===\n");
	while (toklist)
	{
		printf("Token: %s (Type: %d)\n", toklist->value, toklist->type);
		toklist = toklist->next;
	}*/
	/*printf("======================\n\n");
    while (cmd)
    {
        printf("=== Commande ===\n");
        printf("Arguments: ");
        for (int i = 0; i < cmd->argc; i++)
            printf("%s ", cmd->argv[i]);
        printf("\n");
        printf("Fichier d'entrée: %s\n", cmd->infile ? cmd->infile : "NULL");
        printf("Fichier de sortie: %s\n", cmd->outfile ? cmd->outfile : "NULL");
        printf("Mode append: %d\n", cmd->append);
        printf("Heredoc: %d\n", cmd->heredoc);
        printf("Délimiteur heredoc: %s\n", cmd->heredoc_delim ? cmd->heredoc_delim : "NULL");
        printf("Délimiteur suivant: %s\n", cmd->next_deli ? cmd->next_deli : "NULL");
        printf("================\n\n");
        cmd = cmd->next;
    }*/


}