/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:09:43 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/14 15:52:37 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

static int	print_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

static int	print_readonly_error(char *arg)
{
	ft_putstr_fd("minishell: unset: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": readonly variable", STDERR_FILENO);
	return (1);
}

int	builtin_unset(t_cmd *cmd, t_env **env, char **args)
{
	int	i;
	int	status;

	(void)cmd;
	if (!env || !*env || !args)
		return (1);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
			status = print_invalid_identifier(args[i]);
		else if (is_readonly_var(args[i]))
			status = print_readonly_error(args[i]);
		else
			remove_env_var(env, args[i]);
		i++;
	}
	return (status);
}
