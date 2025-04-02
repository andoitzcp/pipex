/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andoitzcp <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:07:28 by andoitzcp         #+#    #+#             */
/*   Updated: 2025/04/02 13:07:53 by andoitzcp        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_cmd	*ft_buildcmdnode(char *scmd)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		handle_error(NULL, "ft_buildcmdnode", errno);
	cmd->argv = ft_split(scmd, ' ');
	cmd->cmd = *(cmd->argv);
	cmd->next = NULL;
	return (cmd);
}

t_cmd	**ft_buildcmdlist(t_cmd **head, char **argv)
{
	t_cmd	*node;

	node = ft_buildcmdnode(argv[2]);
	*head = node;
	node->next = ft_buildcmdnode(argv[3]);
	node = node->next;
	return (head);
}
