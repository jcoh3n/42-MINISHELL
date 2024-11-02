/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 00:37:28 by jcohen            #+#    #+#             */
/*   Updated: 2024/11/02 21:50:10 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static char	*handle_special_var(t_env *env, char special_char)
{
	if (special_char == '?')
		return (ft_itoa(env->last_exit_status));
	return (ft_itoa(getpid()));
}

static int	expand_char(char **result, char c)
{
	char	*new_result;

	new_result = ft_strjoinc(*result, c);
	if (!new_result)
		return (1);
	free(*result);
	*result = new_result;
	return (0);
}

static int	process_var_name(char **result, const char *str, int *i, t_env *env)
{
	char	*var_name;
	char	*env_val;
	int		start;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (start == *i)
	{
		*result = ft_strjoin(*result, "$");
		return (*result == NULL);
	}
	var_name = ft_substr(str, start, *i - start);
	env_val = get_env_value(env, var_name);
	free(var_name);
	if (env_val)
		*result = ft_strjoin(*result, env_val);
	(*i)--;
	return (*result == NULL);
}

static int	handle_var_expansion(char **result, const char *str, int *i,
		t_env *env)
{
	char	*tmp;

	(*i)++;
	if (!str[*i] || ft_isspace(str[*i]))
	{
		*result = ft_strjoin(*result, "$");
		(*i)--;
		return (*result == NULL);
	}
	if (str[*i] == '?' || str[*i] == '$')
	{
		tmp = handle_special_var(env, str[*i]);
		*result = ft_strjoin(*result, tmp);
		free(tmp);
		return (*result == NULL);
	}
	return (process_var_name(result, str, i, env));
}

char	*expand_variables_in_str(char *str, t_env *env, t_quote_type quote_type)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && quote_type != QUOTE_SINGLE)
		{
			if (handle_var_expansion(&result, str, &i, env))
				return (free(result), NULL);
			continue ;
		}
		if (expand_char(&result, str[i]))
			return (free(result), NULL);
	}
	return (result);
}
