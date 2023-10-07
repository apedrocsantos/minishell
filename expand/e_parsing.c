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
    //printf("Result of strjoin: {%s}\n", result);
    return (result);
}

char *search_key(t_data *data, char *str)
{
   // printf("entering search_key\n");
    t_pair *temp;

    temp = data->env;
    while (temp)
    {
        printf("Reading through: %s. searching for %s\n", temp->key, &str[1]);
        if (ft_strchr(str, '$'))
            if (*(str + 1) && !ft_strncmp(&str[1], temp->key, ft_strlen(temp->key)))
            {
                printf("String being sent: {%s%s}\nReturn Value: {%s}\n", &str[1], temp->key, temp->value);
                return (temp->value);
            }
        temp = temp->next;
    }
    return (NULL);
}

char *solve_expansion(t_data *data, char *temp)
{
   // printf("entering solve_expansion\n");
    int i;
    char *final;
    char **trying;

    trying = ft_split(temp, 1);
    int a = -1;
    while (trying[++a] != NULL)
        printf("splitted string: %s\n", trying[a]);
    free(temp);
    final = NULL;
    i = -1;
    while (trying[++i] != NULL)
    {
       // printf("Iterating...\n");
        if (ft_strchr(trying[i], '$'))
        {
            //printf("Found a match\n");
            final = c_strjoin(final, search_key(data, trying[i]));
        }
        else
        {
            //printf("Didnt find a match\n");
            final = c_strjoin(final, trying[i]);
        }
        //printf("Iteration of Final String: %s\n", final);
    }
    free(trying);
    //printf("Final String: %s\n", final);
    return (final);
}

char *format_expansion(t_data *data, t_arg *arg, int i, int c, char flag)
{
    //printf("entering format_expansion()\n");
    char *temp;

    temp = malloc(ft_strlen(arg->token) * 2);
    while (arg->token[++i] != '\0')
    {
        if (arg->token[i] == '$' && arg->token[i - 1])
        {
            flag = arg->token[i];
            temp[c++] = 1;
            temp[c++] = arg->token[i];
        }
        else if (arg->token[i] == '\"')
        {
            temp[c++] = 1;
            temp[c++] = arg->token[i];
        }
        else if (arg->token[i] == ' ' && flag)
        {
            temp[c++] = '=';
            temp[c++] = 1;
            temp[c++] = arg->token[i];
            flag = 0;
        }
        else if ((arg->token[i] == ' ' || arg->token[i] == '\"'))
        {
            temp[c++] = 1;
            temp[c++] = arg->token[i];
        }
        else
            temp[c++] = arg->token[i];
    }
    if (flag)
        temp[c++] = '=';
    temp[c] = '\0';
    free(arg->token);
    return (solve_expansion(data, temp));
}

void    expand_struct(t_data *data, t_command_list *cmd_lst)
{
    //printf("Entering expand_struct()\n");
    print_struct(cmd_lst);
    int i = -1;
    while (cmd_lst)
    {
        while (cmd_lst->arg[++i].token != NULL)
            if (ft_strchr(cmd_lst->arg[i].token, '$'))
                cmd_lst->arg[i].token = format_expansion(data, &(cmd_lst->arg[i]), -1 , 0, 0);
        i = -1;
        cmd_lst = cmd_lst->next;
    }
}