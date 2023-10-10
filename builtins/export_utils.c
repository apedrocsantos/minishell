/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 17:57:33 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/09 21:20:11 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_export_error(t_data *data, char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	data->exit_status = 1;
	return (0);
}

int is_valid(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if(!ft_isalnum(str[i]) || str[i] == ' ')
			return (0);
		i++;
	}
	return (1);
}