/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:54:27 by ggaribot          #+#    #+#             */
/*   Updated: 2024/12/17 20:40:29 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_copy_cmf(char **src, char **dest)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
}

int	add_argument(t_cmd *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			i++;
	}
	new_args = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	if (cmd->args)
	{
		ft_copy_cmf(cmd->args, new_args);
		free(cmd->args);
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
		return (cleanup_ptr(new_args), 0);
	new_args[i + 1] = NULL;
	cmd->args = new_args;
	return (1);
}

static int	handle_heredoc_redirect(t_cmd *cmd, t_token *next)
{
	t_heredoc	*new_heredoc;
	t_heredoc	*current;

	new_heredoc = create_heredoc(next->value);
	if (!new_heredoc)
		return (0);
	if (!cmd->heredocs)
		cmd->heredocs = new_heredoc;
	else
	{
		current = cmd->heredocs;
		while (current->next)
			current = current->next;
		current->next = new_heredoc;
	}
	return (1);
}

static int	handle_redirect_type(t_cmd *cmd, t_token *token, t_token *next,
		t_env *env)
{
	(void)env;
	if (token->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirect(cmd, next));
	if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_APPEND)
		return (handle_output_redirect(cmd, token, next));
	if (token->type == TOKEN_REDIR_IN)
		return (handle_input_redirect(cmd, next));
	return (1);
}

int	handle_redirection(t_cmd *cmd, t_token *token, t_token *next, t_env *env)
{
	if (!next || next->type != TOKEN_WORD)
	{
		print_syntax_error("newline");
		return (0);
	}
	return (handle_redirect_type(cmd, token, next, env));
}
