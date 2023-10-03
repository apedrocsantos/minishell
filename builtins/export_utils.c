/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 17:57:33 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/03 16:37:58 by anda-cun         ###   ########.fr       */
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

int	has_equal(t_pair *env, t_pair *exported_vars, char *str)
{
    t_pair *temp_env;
    t_pair *temp_exp;
    
    temp_env = env;
    temp_exp = exported_vars;
	if (check_dupes(temp_env, str))
		replace_var(temp_env, str);
	else if (check_dupes(temp_exp, str))
		replace_var(temp_exp, str);
	else
		add_to_list(str, temp_exp);
    return (0);
}