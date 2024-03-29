/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_root.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:31:47 by nrea              #+#    #+#             */
/*   Updated: 2024/03/29 11:45:52 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*
execute builtin
l'expansion ifs wildcards et dequotes ont ete faits
On applique les redirections de pipes si pipe_nb != -1
On applique ls redirections
On execute la fonction builtin ( le premier token retourne la fonction a executer, les autres les arguments)
On set l'exit status selon le retour de cette commande
si pipe_nb == -1
	On restaure les fds
	dup2(0, STDIN)
	dup2(1, STDOUT)

pointeur de fonction sous la forme
builtin(T_token *cmd)

il faudrait un tableau de fonction [7] et un fonction int get_builtin qui renvoie
l'index de la fonction a utiliser dans le tableau



*/



/*execute a node (pipe or exec) in a child process and return the exit status
returns -1 in case of failure*/
int	ft_exec_in_fork(t_node *node, int pipe_nb, t_shell *shell)
{
	int	exit_status;
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (node->type == N_PIPE)
			ft_exec_pipe(node, pipe_nb, shell);
		else if (node->type == N_EXEC)
			ft_exec(node, pipe_nb, shell);
	}
	else
	{
		waitpid(pid, &exit_status, 0);
		return (WEXITSTATUS(exit_status));
	}
	return (0);
}

static void	ft_exec_and_or(t_node *tree_root, t_shell *shell)
{
	if (tree_root->type == N_AND)
	{
		ft_exec_root(tree_root->left, shell);
		if (ft_get_exit_status(&shell->shell_vars) == 0)
			ft_exec_root(tree_root->right, shell);
	}
	else if (tree_root->type == N_OR)
	{
		ft_exec_root(tree_root->left, shell);
		if (ft_get_exit_status(&shell->shell_vars) != 0)
			ft_exec_root(tree_root->right, shell);
	}
}

/*The base execution function called at the root of the parse tree*/
int	ft_exec_root(t_node *tree_root, t_shell *shell)
{
	int	exit_status;

	if (tree_root->type == N_EXEC)
	{
		//expansion du node et detection builtin a ajouter
		if (tree_root->is_builtin == 0)
		{
			exit_status = ft_exec_in_fork(tree_root, -1, shell);
			if (exit_status == -1)
				return (-1);
			ft_set_exit_status(exit_status, &shell->shell_vars);
		}
		else
		{
			// execution builtin dans le processus principal// penser a restaurer les fds
		}
	}
	else if (tree_root->type == N_PIPE)
	{
		if (ft_exec_pipe(tree_root, 0, shell) == -1)
			return (-1);
	}
	else
		ft_exec_and_or(tree_root, shell);
	return (1);
}
