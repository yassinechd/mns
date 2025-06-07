#include "../include/minishell.h"

int	ft_strlen_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
        i++;
    return (i);
}

void	ft_strdup_tab(t_minishell **minishell, char **tab)
{
    int i;

    (*minishell)->envp = malloc(sizeof(char *) * (ft_strlen_tab(tab) + 1));
    if (!(*minishell)->envp)
        return ;
    i = 0;
    while (tab[i])
    {
        //printf("path env : %s\n, ", tab[i]);
        (*minishell)->envp[i] = ft_strdup(tab[i]);
        i++;
    }
    (*minishell)->envp[i] = NULL;
}

void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
