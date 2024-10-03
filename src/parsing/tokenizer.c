/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:30:32 by jcohen            #+#    #+#             */
/*   Updated: 2024/10/03 18:01:09 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_token	*ft_create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

t_token_type	get_token_type(char *value)
{
	if (ft_strncmp(value, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(value, ">", 2) == 0)
		return (TOKEN_REDIR_OUTPUT);
	else if (ft_strncmp(value, ">>", 3) == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strncmp(value, "<", 2) == 0)
		return (TOKEN_REDIR_INPUT);
	else if (ft_strncmp(value, "<<", 3) == 0)
		return (TOKEN_HERE_DOC);
	else
		return (TOKEN_WORD);
}

t_token	*ft_tokenizer(char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = handle_quotes(input, i, &head);
		else if (ft_isspace(input[i]))
			i = handle_space(input, i);
		else
			i = handle_word(input, i, &head);
		i++;
	}
	return (head);
}
