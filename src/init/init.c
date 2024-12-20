/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:18:58 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 16:52:50 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_env_node(char *envp)
{
	t_env	*new_node;
	char	*equals_sign;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equals_sign = ft_strchr(envp, '=');
	if (equals_sign)
	{
		new_node->key = ft_substr(envp, 0, equals_sign - envp);
		new_node->value = ft_strdup(equals_sign + 1);
	}
	else
	{
		new_node->key = ft_strdup(envp);
		new_node->value = ft_strdup("");
	}
	if (!new_node->key || !new_node->value)
	{
		cleanup_ptr(new_node->key);
		cleanup_ptr(new_node->value);
		cleanup_ptr(new_node);
		return (NULL);
	}
	return (new_node->next = NULL, new_node);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	t_env	*last_node;

	env = NULL;
	last_node = NULL;
	while (*envp)
	{
		new_node = create_env_node(*envp);
		if (!new_node)
		{
			ft_putendl_fd("minishell: env initialization failed", 2);
			cleanup_all(env, NULL, -1);
			return (NULL);
		}
		if (!env)
			env = new_node;
		else
			last_node->next = new_node;
		last_node = new_node;
		envp++;
	}
	if (env)
		env->last_exit_status = 0;
	return (env);
}

t_env	*initialize_shell(char **envp)
{
	t_env	*env;

	env = init_env(envp);
	if (!env)
		return (NULL);
	return (env);
}
