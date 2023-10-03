/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_unclosed.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:04:01 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/03 15:54:56 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief returns 2 if the string has unclosed quotes or parenthesis;
 *
 * @param str
 * @return int
 */

#include "minishell.h"

int	check_end_of_command(t_data *data, char *str)
{
	int	i;

	i = ft_strlen(str);
	i--;
	while (str[i] && str[i] != '\'' && str[i] != '"')
	{
		while (str[i] == ' ')
			i--;
		if (!str[i])
			break ;
		if (ft_strchr("<>|", str[i]))
		{
			data->exit_status = 2;
			return (print_syntax_error(str[i + 1]));
		}
		break ;
	}
	return (0);
}

int	check_parse_errors(t_data *data, char *str)
{
	char	c;
	int		i;

	i = 0;
	while (str[i])
	{
		c = 0;
		if (token_error(data, str, &i))
			return(0);
		if (str[i])
		{
			c = str[i];
			while (str[i] != c)
				i++;
			if (str[i] && str[i] != c)
			{
				data->exit_status = 2;
				return (print_syntax_error(c));
			}
			else
				break;
		}
		else
			break;
		i++;
	}
	return (check_end_of_command(data, str));
	return (0);
}

// int	main(int ac, char **av)
// {
// 	if (ac > 1)
// 		return (check_unclosed(av[1]));
// }
