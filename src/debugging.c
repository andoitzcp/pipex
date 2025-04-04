/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andoitzcp <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:47 by andoitzcp         #+#    #+#             */
/*   Updated: 2025/04/02 12:51:49 by andoitzcp        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_printcmdnode(t_cmd *cmd)
{
	static int	i = 0;
	int			j;

	ft_printf("Cmd n=%d,\tp=%p\n", i, cmd);
	ft_printf("\tCommand: %s\n", cmd->cmd);
	ft_printf("\tArguments:\n");
	j = 0;
	while (cmd->argv[j])
	{
		ft_printf("\t\t%d-> %s\n", j, cmd->argv[j]);
		j++;
	}
	ft_printf("\tNext cmd: %p\n", cmd->next);
	i++;
}

void	ft_printcmdlist(t_cmd **head)
{
	t_cmd	*node;

	node = *head;
	while (node)
	{
		ft_printcmdnode(node);
		node = node->next;
	}
}
