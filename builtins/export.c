/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 19:59:30 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/03 17:43:58 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_dupes(t_pair *env, char *str)
{
	t_pair	*temp;
	int		str_size;

	if (!env || !env->key)
	{
		return (0);
	}
	temp = env;
	str_size = len_to_char(str, '=');
	while (temp)
	{
		if (!ft_strncmp(str, temp->key, str_size) && (!temp->key[str_size]
				|| temp->key[str_size] == '='))
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	add_to_list(char *str, t_pair *exported_vars)
{
	t_pair	*temp;

	temp = exported_vars;
	if (temp->key)
	{
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_pair));
		temp = temp->next;
	}
	if (!ft_strchr(str, '='))
	{
		temp->key = ft_strdup(str);
		temp->value = NULL;
	}
	else
	{
		temp->key = ft_substr(str, 0, len_to_char(str, '=') + 1);
		temp->value = ft_strdup(ft_strchr(str, '=') + 1);
	}
	temp->next = NULL;
	return (0);
}

int	replace_var(t_pair *env, char *str)
{
	t_pair	*temp;
	char	*temp_str;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(str, temp->key, len_to_char(str, '=')))
		{
			if (!ft_strchr(temp->key, '='))
			{
				temp_str = temp->key;
				temp->key = ft_strjoin(temp_str, "=");
				free(temp_str);
			}
			free(temp->value);
			temp->value = ft_strdup(ft_strchr(str, '=') + 1);
		}
		temp = temp->next;
	}
	return (0);
}

t_pair	*copy_list_all(t_pair *env, t_pair *exported_vars)
{
	t_pair	*temp_env;
	t_pair	*temp_export;

	temp_export = malloc(sizeof(t_pair));
	temp_env = env;
	while (temp_env)
	{
		temp_export->key = ft_strdup(temp_env->key);
		temp_export->value = ft_strdup(temp_env->value);
		temp_env = temp_env->next;
		if (temp_env || exported_vars)
		{
			temp_export->next = malloc(sizeof(t_pair));
			temp_export = temp_export->next;
		}
		else
			temp_export->next = NULL;
	}
	temp_env = exported_vars;
	while (temp_env)
	{
		temp_export->key = ft_strdup(temp_env->key);
		temp_export->value = ft_strdup(temp_env->value);
		temp_env = temp_env->next;
		if (temp_env)
		{
			temp_export->next = malloc(sizeof(t_pair));
			temp_export = temp_export->next;
		}
		else
			temp_export->next = NULL;
	}
	return (temp_export);
}

int	print_sorted_all(t_pair *env, t_pair *exported_vars)
{
	t_pair	*export;
	t_pair	*temp_export;

	export = copy_list_all(env, exported_vars);
	sort_list(export);
	temp_export = export;
	while (temp_export)
	{
		if (ft_strchr(temp_export->key, '='))
			printf("declare -x %s\"%s\"\n", temp_export->key,
				temp_export->value);
		else
			printf("declare -x %s%s\n", temp_export->key, temp_export->value);
		temp_export = temp_export->next;
	}
	free_pairs(export);
	return (0);
}

/*
If string doesn't start with an alphanum char or with a '_', return (error); OK

If string doesn't have a '='
	if string is not in env list && string is not in exported list
		add to exported list

If string has a '=':
	if string is in env list
		replace value
	else if string is not in env list
		if string is in exported list
			replace string
		else
			add to exported vars
 */

int	export(t_data *data, char **str)
{
	t_pair	*env;
	t_pair	*exported_vars;
	int		i;
	int j;

	env = data->env;
	exported_vars = data->exported_vars;
	i = -1;
	if (!*str)
		return (print_sorted_all(env, exported_vars));
	while (str[++i])
	{
		if (!ft_isalpha(*str[i]))
			return (print_export_error(data, str[i]));
		j = -1;
		while (str[i][++j])
		{
			if (!ft_isalnum(str[i][j]) && str[i][j] != '_' && str[i][j] != '=')
				return (print_export_error(data, str[i]));
		}
		if (!ft_strchr(str[i], '=') && (!check_dupes(env, str[i])
				&& !check_dupes(exported_vars, str[i])))
			return (add_to_list(str[i], exported_vars));
		if (ft_strchr(str[i], '=') && check_dupes(env, str[i]))
			return (replace_var(env, str[i]));
		if (ft_strchr(str[i], '=') && check_dupes(exported_vars, str[i]))
			return (replace_var(exported_vars, str[i]));
		if (ft_strchr(str[i], '='))
			return (add_to_list(str[i], exported_vars));
	}
	return (0);
}
