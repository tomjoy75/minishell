/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_repl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:39:35 by nrea              #+#    #+#             */
/*   Updated: 2024/04/10 10:48:34 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_tokenisation(char *line, t_token **stack, t_shell *shell)
{
	int	err_code;

	err_code = 0;
	err_code = tokenise(line, stack);
	if (err_code)
	{
		ft_dprintf(2, "tokenisation error: %s\n", ft_handle_tok_err(err_code));
		kill_stack(stack);
		*stack = NULL;
		free(line);
		ft_set_exit_status(2, &shell->shell_vars);
		return (-1);
	}
	free(line);
	return (0);
}

static int	apply_check_syntax(t_token **stack, t_shell *shell)
{
	int	error_code;

	error_code = check_syntax(*stack);
	if (error_code)
	{
		kill_stack(stack);
		*stack = NULL;
		ft_set_exit_status(2, &shell->shell_vars);
		return (-1);
	}
	return (0);
}

static int	apply_redirections(t_token **stack, t_shell *shell)
{
	int ret;

	ret = ft_redirections(stack);
	if (ret == -1)
	{
		write(2, "Internal error during redirection\n", 35);
		kill_stack(stack);
		*stack = NULL;
		ft_set_exit_status(2, &shell->shell_vars);
		return (-1);
	}
	else if (ret == 2)
	{
		kill_stack(stack);
		*stack = NULL;
		return (-1);
	}
	return (0);
}

static int	apply_create_tree(t_token **stack, t_shell *shell)
{
	int		error_code;

	error_code = 0;
	shell->tree = ft_create_tree(stack, &error_code, 2);
	if (error_code || !shell->tree)
	{
		ft_handle_tree_error(error_code);
		kill_stack(stack);
		ft_free_tree(&shell->tree);
		return (-1);
	}
	return (0);
}

/*The main Read Execute Print Loop*/
void	repl(t_shell *shell)
{
	t_token	*stack;
	char	*line;

	line = NULL;
	while (1)
	{
		stack = NULL;
		line = readline("minishell>");
		if (g_sig)
		{
			if (g_sig == SIGINT)
			{
				ft_set_exit_status(g_sig + 128, &shell->shell_vars);
				g_sig = 0;
			}
			else
				g_sig = 0;
		}
		if (!line)
			exit_gracefully(shell, 0);
		if (ft_strlen(line))
		{
			add_history(line);
			if (apply_tokenisation(line, &stack, shell) == -1)
				continue ;
			if (apply_check_syntax(&stack, shell) == -1)
				continue ;
			if (apply_redirections(&stack, shell) == -1)
				continue ;
			if (apply_create_tree(&stack, shell) == -1)
				continue ;
			if (ft_exec_root(shell->tree, shell) == -1)
				return ;
			ft_free_tree(&shell->tree);
		}
		if (ft_get_exit_status(&shell->shell_vars) == 130)
			printf("\r");
		if (ft_get_exit_status(&shell->shell_vars) == 131)
			printf("\r^\\Quit\033[K\n");
	}


}
