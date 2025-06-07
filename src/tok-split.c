#include "../include/minishell.h"

char	**ft_free_split(char **tab)
{
	int	i;

	if (!tab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

static int	char_len(const char *str)
{
	if ((str[0] == '<' && str[1] == '<')
		|| (str[0] == '>' && str[1] == '>'))
		return (2);
	if (str[0] == '<' || str[0] == '>'
		|| str[0] == '|')
		return (1);
	return (0);
}

static int	count_token(const char *s)
{
	int i;
	int count;
	int len;
	char c;

	c = 0;
	i = 0;
	count = 0;
	while (s[i])
	{
		while (is_blank(s[i]))
			i++;
		if (!s[i])
			break ;
		len = char_len(s + i);
		if (len)
			i += len;
		else if (s[i] == '\'' || s[i] == '"')
		{
			c = s[i++];
			while (s[i] && s[i] != c)
				i++;
			if (s[i] == c)
				i++;
		}
		else
		{
			while (s[i] && !is_blank(s[i]) && !char_len(s + i)
				&& s[i] != '\'' && s[i] != '"')
				i++;
		}
		count++;
	}
	return (count);
}

static char	*extract_token(const char *s, int *i)
{
	int start;
	int len;
	char c;

	c = 0;
	while (is_blank(s[*i]))
		(*i)++;
	start = *i;
	len = char_len(s + *i);
	if (len)
		*i += len;
	else if (s[*i] == '\'' || s[*i] == '"')
	{
		c = s[(*i)++];
		while (s[*i] && s[*i] != c)
			(*i)++;
		if (s[*i] == c)
			(*i)++;
	}
	else
	{
		while (s[*i] && !is_blank(s[*i]) && !char_len(s + *i)
			&& s[*i] != '\'' && s[*i] != '"')
			(*i)++;
	}
	len = *i - start;
	return (ft_substr(s, start, len));
}

char	**ft_split_tokens(char *s)
{
	char	**tab;
	int		count;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	count = count_token(s);
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count)
	{
		tab[j] = extract_token(s, &i);
		if (!tab[j])
			return (ft_free_split(tab), NULL);
		j++;
	}
	tab[j] = NULL;
	return (tab);
}
