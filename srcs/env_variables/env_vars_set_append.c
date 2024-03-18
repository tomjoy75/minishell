/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_set_append.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:48:02 by nrea              #+#    #+#             */
/*   Updated: 2024/03/18 17:50:20 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_variables.h"
#include "libft.h"
#include <stdio.h>

/*allocate a var structure an initialize it with key and value*/
static t_evar	*ft_create_var(char *key, char *value)
{
	t_evar	*var;

	if (!key || !key[0])
		return (NULL);
	var = NULL;
	var = malloc(sizeof(t_evar));
	if (!var)
		return (NULL);
	var->key = NULL;
	ft_memset(var, 0, sizeof(var));
	var->key = ft_strdup(key);
	if (!var->key)
		return (ft_free_var(var));
	var->value = NULL;
	var->next = NULL;
	if (value)
	{
		var->value = ft_strdup(value);
		if (!var->value)
			return (ft_free_var(var));
	}
	return (var);
}

/*get the value of the var identified by key
if the var doesn't exists  -> returns null */
static t_evar	*ft_get_var(char *key, t_evar *sub_list)
{
	t_evar	*var;

	if (!ft_is_valid_key(key))
		return (NULL);
	var = NULL;
	var = sub_list;
	while (var)
	{
		if (!ft_strcmp(key, var->key))
			return (var);
		var = var->next;
	}
	return (NULL);
}

/*create a new var and place it at the right ascii order in the right stack*/
static int	ft_create_and_place(char *key, char *value, t_evar **env_list)
{
	t_evar	*var;
	t_evar	*head;
	t_evar	*prev;

	var = NULL;
	var = ft_create_var(key, value);
	if (!var)
		return (0);
	head = *env_list;
	if (!head || ft_strcmp(head->key, key) > 0)
	{
		*env_list = var;
		var->next = head;
	}
	else
	{
		while (head && ft_strcmp(head->key, key) < 0)
		{
			prev = head;
			head = head->next;
		}
		var->next = head;
		prev->next = var;
	}
	return (1);
}

/*if the var exists => modify its value
else : create a new var and place it at the right spot*/
int	ft_set_var(char *key, char *value, t_evar **env_list)
{
	t_evar	*var;
	int		index;
	char	*val;

	val = NULL;
	if (!ft_is_valid_key(key))
		return (0);
	index = key[0] - 65;
	if (index < 0 || index > 57)
		return (0);
	var = ft_get_var(key, env_list[index]);
	if (!var)
		return (ft_create_and_place(key, value, &env_list[index]));
	else
	{
		if (value)
		{
			val = ft_strdup(value);
			if (!val)
				return (0);
		}
		free((var)->value);
		(var)->value = val;
	}
	return (1);
}

/*Similar to ft_set_var but append the value at the end of the var value
like the export builtin with +=  */
int	ft_append_var(char *key, char *value, t_evar **env_list)
{
	t_evar	*var;
	int		index;
	char	*or_val;
	char	*new_val;

	new_val = NULL;
	if (!ft_is_valid_key(key))
		return (0);
	index = key[0] - 65;
	if (index < 0 || index > 57)
		return (0);
	var = ft_get_var(key, env_list[index]);
	if (!var)
		return (ft_create_and_place(key, value, &env_list[index]));
	else
	{
		or_val = ft_get_var_value(key, env_list);
		new_val = ft_strjoin(or_val, value);
		if (!new_val)
			return (0);
		if (var->value)
			free((var)->value);
		(var)->value = new_val;
	}
	return (1);
}
