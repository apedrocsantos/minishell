/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 19:09:01 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/11 11:00:36 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_items(int len, char **env_list, t_pair *env, t_pair *exported_vars)
{
	int	i;

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
}

char	**get_env_list(t_pair *env, t_pair *exported_vars)
{
	char	**env_list;
	int		len;
	t_pair	*temp_env;
	t_pair	*temp_exported;

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
	copy_items(len, env_list, env, exported_vars);
	return (env_list);
}

void	child_error(t_data *data, t_command_list *cmd_lst, char **args,
		char **env_list)
{
	int	i;

	i = 0;
	if (data->pipes.open)
		close(data->pipes.fd[0]);
	while (env_list[i])
		free(env_list[i++]);
	revert_fds(cmd_lst);
	if (args && *args)
		ft_putstr_fd(*args, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

void	free_pid(t_data *data)
{
	t_pid	*temp;

	while (data->pid->value != 0)
	{
		temp = data->pid;
		data->pid = data->pid->next;
		free(temp);
	}
	data->pid->value = 0;
}

void	add_pid(t_data *data, int pid, t_command_list *cmd_lst)
{
	t_pid	*temp;

	temp = malloc(sizeof(t_pid));
	temp->value = pid;
	if (!cmd_lst->next)
		temp->last = 1;
	else
		temp->last = 0;
	temp->next = data->pid;
	data->pid = temp;
}
