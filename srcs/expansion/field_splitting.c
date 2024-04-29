/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:56:40 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/04/29 15:54:47 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_expansion.h"

// Create and add new tokens in a linked list
// Add content from a tab of char*
// Return 1 if error
int	words_to_token(t_token *ptr, char **words)
{
	t_token	*new;

	words++;
	while (*words)
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return (1);
		ft_memset(new, 0, sizeof(t_token));
		new->param_expanded = 1;
		new->content = ft_strdup(*words);
		new->next = ptr->next;
		ptr->next = new;
		ptr = ptr->next;
		words++;
	}
	return (0);
}

void	free_words_tab(char ***words)
{
	char	**original_words;

	original_words = *words;
	while (**words)
	{
		free(**words);
		**words = NULL;
		(*words)++;
	}
	free(original_words);
	original_words = NULL;
}

int	get_last_word(char **words)
{
	int	i;

	if (!*words)
		return (-1);
	i = 0;
	while (words[i] != NULL)
		i++;
	return (i - 1);
}
// Gestion de l'IFS apres l'expansion des parametres
// Return 0 ou code erreur
int	field_splitting(t_token *token, t_shell *shell, char *new, char *start, char *end)
{
	char	*ifs;
	char	**words;
	t_token	*ptr;
	int		last;
	char	*tmp;
	char	*tmp2;



	ifs = ft_get_var_value("IFS", shell->env_vars, shell->shell_vars);
	words = ft_split_multiple(new, ifs);
	ptr = token;
	if (words == NULL)
		return (1);
	if (words[0])
	{
		last = get_last_word(words);
		if (last == -1)
			return (free(words), 0);
		tmp2 = ft_strjoin(start, words[0]); // a proteger
		tmp = ft_strjoin(words[last], end); // a proteger
		free (words[last]);
		words[last] = tmp;
	}
	else
	{
		free_words_tab(&words);
		words = malloc(3 * sizeof(char*)); // a proteger
		tmp2 = ft_strdup(start);
		words[0] = ft_strdup("");
		words[1] = ft_strdup(end);
		words[2] = NULL;
	}
	words_to_token(ptr, words); // a proteger
	free_words_tab(&words);
	free(token->content);
	token->content = tmp2;
	return (0);
}
