/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:54:26 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/17 16:09:59 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/execution.h"

void	update_pwd(t_env *env, char *old_pwd, char *new_pwd)
{
	if (!env || !old_pwd)
	{
		free(old_pwd);
		free(new_pwd);
		return ;
	}
	if (new_pwd)
	{
		if (custom_setenv(&env, "OLDPWD", old_pwd) == -1)
			ft_putendl_fd("minishell: failed to update OLDPWD", 2);
		if (custom_setenv(&env, "PWD", new_pwd) == -1)
			ft_putendl_fd("minishell: failed to update PWD", 2);
	}
	else
	{
		ft_putendl_fd("minishell: failed to get new working directory", 2);
	}
	free(old_pwd);
	free(new_pwd);
}
