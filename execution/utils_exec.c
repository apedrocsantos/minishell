/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 10:04:18 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/10 11:40:19 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_arg_list(t_arg *arg)
{
	int		len;
	int		i;
	int		j;
	char	**arg_list;

	len = 0;
	i = 0;
	while (arg[i].token != NULL)
	{
		if (arg[i].type == 0)
			len++;
		i++;
	}
	if (len)
		arg_list = (char **)ft_calloc(len + 1, sizeof(char *));
	else
		return (NULL);
	i = -1;
	j = 0;
	while (arg[++i].token != NULL)
	{
		if (arg[i].type == 0)
			arg_list[j++] = arg[i].token;
	}
	return (arg_list);
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

char	**get_path(t_data *data)
{
	t_pair	*temp;

	if (data->path)
	{
		free_path(data->path);
		data->path = NULL;
	}
	temp = data->env;
	while (temp)
	{
		if (!ft_strncmp(temp->key, "PATH=", 5))
			return (ft_split(temp->value, ':'));
		temp = temp->next;
	}
	temp = data->exported_vars;
	while (temp)
	{
		if (temp && temp->key && !ft_strncmp(temp->key, "PATH=", 5))
			return (ft_split(temp->value, ':'));
		temp = temp->next;
	}
	return (NULL);
}

int	check_path(t_data *data, t_command_list *cmd_lst, char **str, int i)
{
	char	*temp;
	char	*path_to_test;
	char	**path;

	path_to_test = NULL;
	data->path = get_path(data);
	path = data->path;
	if (!path || !*path || !str)
		path_to_test = ft_strdup("");
	if (**str == '/')
		path_to_test = ft_strdup(*str);
	if (path_to_test)
		return (0);
	while (path[++i])
	{
		temp = ft_strjoin(path[i], "/");
		path_to_test = ft_strjoin(temp, *str);
		free(temp);
		if (access(path_to_test, X_OK | F_OK) == 0)
			break ;
		else if (path[i + 1])
			free(path_to_test);
	}
	cmd_lst->exec_path = path_to_test;
	return (0);
}
