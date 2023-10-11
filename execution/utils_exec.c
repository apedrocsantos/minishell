/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 10:04:18 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/11 10:56:34 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_args(t_arg *arg, char **arg_list)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (arg[++i].token != NULL)
	{
		if (arg[i].type == STR || arg[i].type == EXEC)
			arg_list[j++] = arg[i].token;
	}
	return (arg_list);
}

char	**get_arg_list(t_data *data, t_arg *arg)
{
	int		len;
	int		i;
	char	**arg_list;

	len = 0;
	i = 0;
	while (arg[i].token != NULL)
	{
		if (arg[i].type == EXEC && (access(arg[i].token, X_OK | F_OK) != 0))
		{
			print_file_error("minishell: ", arg[i].token);
			if (errno == 2)
				errno = 127;
			data->exit_status = errno;
			arg[i].type = -20;
		}
		if (arg[i].type == STR || arg[i].type == EXEC)
			len++;
		i++;
	}
	if (len)
		arg_list = (char **)ft_calloc(len + 1, sizeof(char *));
	else
		return (NULL);
	return (get_args(arg, arg_list));
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
	if (!temp)
		return (NULL);
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

	path_to_test = NULL;
	data->path = get_path(data);
	if (!data->path || !*data->path || !str)
		path_to_test = ft_strdup("");
	else if (!ft_strncmp("./", *str, 2) || !ft_strncmp("/", *str, 1))
		path_to_test = ft_strdup(*str);
	if (!path_to_test)
	{
		while (data->path[++i])
		{
			temp = ft_strjoin(data->path[i], "/");
			path_to_test = ft_strjoin(temp, *str);
			free(temp);
			if (access(path_to_test, X_OK | F_OK) == 0)
				break ;
			else if (data->path[i + 1])
				free(path_to_test);
		}
	}
	cmd_lst->exec_path = path_to_test;
	return (0);
}
