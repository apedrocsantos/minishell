/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 16:34:33 by anda-cun          #+#    #+#             */
/*   Updated: 2023/09/30 18:19:10 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void expand_str(char **str)
{
	int i;

	i = 0;
	printf("%s\n", *str);
	while (*str[i] != ' ')
		i++;
	// printf("%s\n", str[i]);
	// if (!str[i])
	// 	return;
}

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
	while (split[++i])
		expand_str(&split[i]);
}