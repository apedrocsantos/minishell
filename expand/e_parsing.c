/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_parsing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 11:45:55 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/08 12:16:10 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *c_strjoin(char *str, char *s)
{
    int i = -1;
    int j = 0;
    char *result = malloc(ft_strlen(str) + ft_strlen(s) + 1);
    if(!str)
        return (s);
    if (!s)
        return (str);
    while (str[++i] != '\0')
        result[j++] = str[i];
    i = -1;
    while (s[++i] != '\0')
        result[j++] = s[i];
    result[j] = '\0';
    free(str);
    //free(s);
    return (result);
}

char *search_key(t_data *data, char *str)
{
    t_pair *temp;

    temp = data->env;
    while (temp)
    {
        if (ft_strchr(str, '$'))
            if (*(str + 1) && !ft_strncmp(&str[1], temp->key, ft_strlen(&str[1])))
                return (temp->value);
        temp = temp->next;
    }
    return (NULL);
}

char *solve_expansion(t_data *data, char *temp)
{
    int i;
    char *final;
    char **trying;

    trying = ft_split(temp, 1);
    free(temp);
    final = NULL;
    i = -1;
    while (trying[++i] != NULL)
        if (ft_strchr(trying[i], '$'))
            final = c_strjoin(final, search_key(data, trying[i]));
        else
            final = c_strjoin(final, trying[i]);
    free(trying);
    return (final);
}

char    *format_expansion(t_data *data, t_arg *arg, int i, int c)
{
    char *temp;

    temp = malloc(ft_strlen(arg->token) * 2);
    while (arg->token[++i] != '\0')
    {
        if (arg->token[i] == '$' && arg->token[i - 1])
        {
            temp[c++] = 1;
            temp[c++] = arg->token[i];
        }
        else if (arg->token[i] == '\"' || arg->token[i] == '\'' || arg->token[i] == ' ')
        {
            temp[c++] = 1;
            temp[c++] = arg->token[i];
            temp[c++] = 1;
        }
        else
        temp[c++] = arg->token[i];
    }
    temp[c] = '\0';
   //free(arg->token);
   return (solve_expansion(data, temp));
}

void    expand_struct(t_data *data, t_command_list *cmd_lst)
{
    int i = -1;
    while (cmd_lst)
    {
        while (cmd_lst->arg[++i].token != NULL)
            if (ft_strchr(cmd_lst->arg[i].token, '$'))
                cmd_lst->arg[i].token = format_expansion(data, &(cmd_lst->arg[i]), -1 , 0);
        i = -1;
        cmd_lst = cmd_lst->next;
    }
}
