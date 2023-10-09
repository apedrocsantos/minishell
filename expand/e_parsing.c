/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_parsing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:18:30 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/09 17:18:31 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char    *c_strdup(const char *s, char *freeit)
{
    char    *str;
    int     length;
    int     i;
    length = ft_strlen(s);
    i = -1;
    str = (char *)malloc(sizeof(char) * (length + 1));
    free(freeit);
    if (!str)
        return (NULL);
    while (s && s[++i])
        str[i] = s[i];
    str[length] = '\0';
    return (str);
}
char *c_strjoin(char *str, char *s)
{
    int i = -1;
    int j = 0;
    char *result;
    if(!str)
        result = ft_strdup(s);
    else if (!s)
        result = ft_strdup(str);
    else
    {
        result = malloc(ft_strlen(str) + ft_strlen(s) + 1);
        while (str[++i] != '\0')
            result[j++] = str[i];
        i = -1;
        while (s[++i] != '\0')
            result[j++] = s[i];
        result[j] = '\0';
    }
    free(str);
    if (s != NULL)
        free(s);
    return (result);
}
char    *ft_append(char *s, char c)
{
    char *temp;
    int i;
    i = -1;
    temp = malloc(ft_strlen(s) + 2);
    while (s[++i] != '\0')
        temp[i] = s[i];
    temp[i++] = c;
    temp[i] = '\0';
    free(s);
    return (temp);
}
char *search_key(t_data *data, char *str)
{
    t_pair *temp;
    char    *temp1;
    temp = data->env;
    if (!*(str + 1))
        return (c_strdup("$", str));
    temp1 = ft_append(str, '=');
    while (temp)
    {
        if (*(temp1 + 1) && !ft_strncmp(&temp1[1], temp->key, ft_strlen(&temp1[1])))
            return (c_strdup(temp->value, temp1));
        else if (*(temp1 + 1) && *(temp1 + 1) == '?')
        {
            free(temp1);
            return (ft_itoa(data->exit_status));
        }
        temp = temp->next;
    }
    free(temp1);
    return (NULL);
}
int parse(char **trying, int i, char *flag)
{
    if (*flag)
    {
        if (trying[i][0] != *flag)
            return (1);
        else
        {
            *flag = 0;
            return (0);
        }
    }
    else
    {
        if (trying[i][0] == '\"' || trying[i][0] == '\'')
        {
            *flag = trying[i][0];
            return (0);
        }
        else
            return (1);
    }
}
char *solve_expansion(t_data *data, char *temp, int c)
{
    int i;
    char *final;
    char **trying;
    char flag;
    flag = 0;
    temp[c] = '\0';
    trying = ft_split(temp, 1);
    free(temp);
    final = NULL;
    i = -1;
    while (trying[++i] != NULL)
    {
        if (ft_strchr(trying[i], '$') && flag != '\'')
            final = c_strjoin(final, search_key(data, trying[i]));
        else if(parse(trying, i, &flag))
            final = c_strjoin(final, trying[i]);
        else
        {
            final = c_strjoin(final, NULL);
            free(trying[i]);
        }
    }
    free(trying);
    return (final);
}
char    *format_expansion(t_data *data, t_arg *arg, int i, int c)
{
    char *temp;
    temp = malloc(ft_strlen(arg->token) * 3 + 1);
    while (arg->token[++i] != '\0')
    {
        if ( i > 0 && arg->token[i - 1] == '$' && arg->token[i] == '?')
            {
                temp[c++] = arg->token[i];
                temp[c++] = 1;
            }
        else if (arg->token[i] == '$' && i > 0)
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
   return (solve_expansion(data, temp, c));
}
void    expand_struct(t_data *data, t_command_list *cmd_lst)
{
    int i = -1;
    char *temp;
    while (cmd_lst)
    {
        while (cmd_lst->arg[++i].token != NULL)
        {
            temp = cmd_lst->arg[i].token;
            cmd_lst->arg[i].token = format_expansion(data, &(cmd_lst->arg[i]), -1 , 0);
            if (!ft_strncmp(cmd_lst->arg[i].token, "", ft_strlen(cmd_lst->arg[i].token)) && temp[0] == '$')
                cmd_lst->arg[i].type = -10;
            free(temp);
        }
        i = -1;
        cmd_lst = cmd_lst->next;
    }
}
