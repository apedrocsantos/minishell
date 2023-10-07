/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:41:40 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/05 10:08:47 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	write_string(char **str, int newline)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (ft_putstr_fd(str[j], 1) == -1)
			return (-1);
		if (str[j + 1])
			if (ft_putchar_fd(' ', 1) == -1)
				return (-1);
		j++;
	}
	if (newline)
		if (ft_putendl_fd("", 1) == -1)
			return (-1);
	return (0);
}

int	echo(t_data *data, char **str)
{
	int	i;
	int	j;
	int	newline;

	i = 0;
	j = 0;
	newline = 1;
	if (!*str)
		return (ft_putendl_fd("", 1));
	if (!ft_strncmp(str[0], "$?", 2))
	{
		ft_putnbr_fd(data->exit_status, 1);
		ft_putendl_fd("", 1);
		data->exit_status = 0;
		return (0);
	}
	if (!ft_strncmp(str[0], "-n", 2))
	{
		while (str[0][1 + i] == 'n')
			i++;
		if (!str[0][1 + i])
		{
			newline = 0;
			j++;
		}
	}
	data->exit_status = write_string(&str[j], newline);
	return (0);
}

// int main(int ac, char **av)
// {
//     if (ac == 2)
//         return(echo(av[1]));
// }