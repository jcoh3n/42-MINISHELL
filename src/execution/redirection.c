/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:39:24 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/25 18:10:20 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	setup_fd_backup(int *prev_in_fd, int *prev_out_fd)
{
	*prev_in_fd = dup(STDIN_FILENO);
	*prev_out_fd = dup(STDOUT_FILENO);
	if (*prev_in_fd == -1 || *prev_out_fd == -1)
	{
		perror("minishell: dup");
		return (0);
	}
	return (1);
}

static int	handle_input_redirection(t_cmd *cmd, int *prev_fds)
{
	if (!cmd->input_file)
		return (1);
	if (!check_input_permissions(cmd, prev_fds))
		return (0);
	cmd->input_fd = open(cmd->input_file, O_RDONLY);
	if (cmd->input_fd == -1 || dup2(cmd->input_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: input redirection");
		cleanup_fds(prev_fds[0], prev_fds[1]);
		return (0);
	}
	close(cmd->input_fd);
	return (1);
}

static int	handle_output_redirection(t_cmd *cmd, int *prev_fds)
{
	int	flags;

	if (!cmd->output_file)
		return (1);
	if (cmd->append_output)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (!check_output_permissions(cmd, prev_fds))
		return (0);
	if (!setup_output_fd(cmd, flags, prev_fds))
		return (0);
	return (1);
}

int	setup_redirections(t_cmd *cmd)
{
	int	prev_fds[2];
	int	ret;

	if (!setup_fd_backup(&prev_fds[0], &prev_fds[1]))
		return (0);
	if (!handle_input_redirection(cmd, prev_fds))
		return (0);
	ret = handle_output_redirection(cmd, prev_fds);
	cleanup_fds(prev_fds[0], prev_fds[1]);
	return (ret);
}
