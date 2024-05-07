/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:08:35 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/07 11:23:29 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H
# include <stdlib.h>
# include <stdio.h>
# include "libft.h"
# include "minishell_signals.h"

# define PIPE 1 // "|"
# define AND 2 // "&&"
# define OR 3 // "||"
# define R_IN 4 // "<"
# define R_OUT 5 // ">"
# define R_APPEND 6 // ">>"
# define R_HEREDOC 7 // "<<"
# define O_BRACKET 8 // "("
# define C_BRACKET 9 // ")"
# define WORD 0 // Tout le reste
# define INVALID -1 // ";" "&" "\"

typedef struct s_token
{
	int				type;
	char			*content;
	int				nb_files;
	int				hidden;
	int				param_expanded;
	int				path_expanded;
	int				heredoc_quoted;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

t_token	*ft_get_token(t_token *stack, int rank);
int		ft_stack_size(t_token *stack);
int		ft_free_token(t_token **token);
void	ft_reverse_stack(t_token **stack);

int		tokenise(char *str, t_token **stack);
int		check_syntax(t_token *stack);
// token_utils2
int		is_wspace(char c);
int		is_operator(char c);
void	kill_stack(t_token **stack);
t_token	*new_token(char *str, int len);
t_token	*add_token(t_token *stack, t_token *new);
int		word_is_wspace(char *str);
int		contains_quotes(char *str);

#endif
