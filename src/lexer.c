#include "../include/minishell.h"

void	ft_clear_tokens(t_token **lst)
{
	t_token	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->value);
		free(*lst);
		*lst = tmp;
	}
}

static int	syntax_error(t_token *lst)
{
	// ajouter toute les autre syntax erreur possible et les gerer ici avant de les gerer dans le token
	if (!lst)
		return (1);
	if (lst->type == 0) /* pipe en premier */
		return (ft_printf("syntax error\n"), 1);
	while (lst)
	{
		if (lst->type == 0 && (!lst->next || lst->next->type == 0))
			return (ft_printf("syntax error\n"), 1);
		lst = lst->next;
	}
	return (0);
}
t_token	*lexer(char *prompt)
{
	char	**tab;
	t_token	*tokens;

	if (!prompt)
		return (NULL);
	tab = ft_split_tokens(prompt);
	if (!tab)
		return (NULL);
	tokens = build_list(tab);
	ft_free_split(tab);
	if (!tokens || syntax_error(tokens))
	{
		ft_clear_tokens(&tokens);
		return (NULL);
	}
	return (tokens);

}