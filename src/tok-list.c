#include "../include/minishell.h"

int	get_type(const char *tok)
{
	if (!tok || !*tok)
		return (5);
	if (tok[0] == '|' && !tok[1])
		return (0);
	if (tok[0] == '>' && tok[1] == '>' && !tok[2])
		return (1);
	if (tok[0] == '<' && tok[1] == '<' && !tok[2])
		return (2);
	if (tok[0] == '<' && !tok[1])
		return (3);
	if (tok[0] == '>' && !tok[1])
		return (4);
	return (5); /* Les guillemets restent un mot complet */
}


static t_token	*new_token(const char *str)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(str);
	if (!tok->value)
		return (free(tok), NULL);
	tok->type = get_type(str);
	tok->next = NULL;
	return (tok);
}


t_token	*build_list(char **tab)
{
	t_token	*head;
	t_token	*cur;
	int		i;

	if (!tab || !tab[0])
		return (NULL);
	head = new_token(tab[0]);
	if (!head)
		return (NULL);
	cur = head;
	i = 1;
	while (tab[i])
	{
		cur->next = new_token(tab[i]);
		if (!cur->next)
		{
            //remplplacer par une fonction qui free toute ma liste en cas d'erreur
			while (head)
			{
				t_token *tmp = head->next;
				free(head->value);
				free(head);
				head = tmp;
			}
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	return (head);
}