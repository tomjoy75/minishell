/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_expansion.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 14:28:30 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/04/07 16:36:18 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_EXPANSION_H
# define WORD_EXPANSION_H
# include <dirent.h>
# include "libft.h"
# include "parser.h"
# include "env_variables.h"
# include "minishell.h"

int		field_splitting(t_token *token, t_shell *shell);
char	*expand_param(char *str, t_shell *shell);
int		words_to_token(t_token **ptr, char **words);
int		field_splitting(t_token *token, t_shell *shell);
void	free_words_tab(char ***words);
int		expand_pathname_cmd(t_token *token, int *count);
int 	expand_pathname_redir(t_token *token);
#endif
