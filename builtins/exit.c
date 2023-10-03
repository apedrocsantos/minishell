/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:07:29 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/03 12:28:35 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exit_builtin(t_data *data, char **str)
{
	unsigned char ret;
	int i;

	// if (!data->pipes.open)
	// {
	// 	ft_putendl_fd("exit", 2);
	// 	data->exit = 1;
	// }
	// else
	// 	data->exit = 0;
	if (!str || !*str)
		return (data->exit_status);
	i = -1;
	while (str[0][++i])
		if (!ft_isdigit(str[0][i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(str[0], 2);
			ft_putendl_fd(": numeric argument required", 2);
			data->exit_status = 2;
			return (data->exit_status);
		}
	ret = ft_atoi(str[0]);
	data->exit_status = ret;
	return (data->exit_status);
}
