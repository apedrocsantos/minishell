/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 16:34:33 by anda-cun          #+#    #+#             */
/*   Updated: 2023/09/30 16:55:59 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand(t_arg *arg)
{
	int i;
	char *str;
	char **split;

	i = 0;
	if (!arg || arg->type != STR)
		return ;
	str = arg->token;
	split = ft_split(str, '$');
	if (!split[1])
		return;
	while (split[i])
		printf("%s\n", split[i++]);
}