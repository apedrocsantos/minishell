/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 19:09:01 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/10 19:09:48 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env_list(t_pair *env, t_pair *exported_vars)
{
	char	**env_list;
	int		len;
	t_pair	*temp_env;
	t_pair	*temp_exported;
	int		i;

	temp_env = env;
	temp_exported = exported_vars;
	len = 0;
	while (temp_env)
	{
		temp_env = temp_env->next;
		len++;
	}
	while (temp_exported)
	{
		temp_exported = temp_exported->next;
		len++;
	}
	env_list = ft_calloc(len + 1, sizeof(t_pair));
	env_list[len] = NULL;
	i = 0;
	while (i < len)
	{
		if (!env)
			break ;
		env_list[i] = ft_strjoin(env->key, env->value);
		env = env->next;
		i++;
	}
	while (i < len)
	{
		if (!exported_vars->key)
			break ;
		env_list[i] = ft_strjoin(exported_vars->key, exported_vars->value);
		exported_vars = exported_vars->next;
		i++;
	}
	return (env_list);
}
