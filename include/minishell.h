/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:19:14 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/03/15 13:30:56 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "parser.h"
# include "token.h"
# include "token_utils.h"
# include "libft.h"
# include "env_variables.h"



typedef struct	s_shell
{
t_evar	*env_variables[58];
char	*ifs; // a initialiser avec " \n"
int		exit_code; // a initialiser a 0


}	t_shell;
#endif
