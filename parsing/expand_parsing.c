#include "minishell.h"

char *match_env(t_data *data, char *key)
{
    char *temp;
    temp = ft_strjoin(key, "=");
    printf("FINAL KEY: {%s}\n", temp);
    while (data->env->next != NULL)
    {
        if (!ft_strncmp(data->env->key, temp, ft_strlen(temp)))
        {
            printf("INSIDE CONDITION\n");
            free(temp);
            printf("ENV: {%s}; KEY: {%s}\n", data->env->value, data->env->key);
            return (data->env->value);
        }
        data->env = data->env->next;
    }
    free(temp);
    free(key);
    return (NULL);
}

size_t	ft_strlena(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] && s[i] != '$' && s[i] != ' ')
		i++;
	return (i);
}

char	*ft_strjoini(char *s1, char *s2, int j)
{
    printf("String sent to ft_strjoini: %s\n", s1);
	int		size;
	int		i;
	char	*str;
    int c;

    c = 0;
	i = 0;
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
    printf("Size of var: %d\n", size);
	str = malloc(size);
    if (!s2)
    {
        str = s1;
        free(s1);
        return (str);
    }
    if (!str)
		return (NULL);
	while (s1[i] && s1[i] != ' ')
	{
        printf("placing char: {%c} in string: {%s}\n", s1[i], str);
		str[c++] = s1[i];
		i++;
	}
	while (s2[j])
	{
        printf("placing char: {%c} in string: {%s}\n", s2[j], str);
		str[c++] = s2[j];
		j++;
	}
	str[i] = '\0';
	return (str);
}

void expand_var(t_data *data, char *splitter, int i, int c)
{
    printf("INSIDE EXPAND_VAR()\n");
    char **temp;
    char *stemp;

    temp = ft_split(splitter, '$');
    while (temp[i])
    {
        stemp = temp[i];
        if (temp[i][0] == '$')
        {
            while (temp[i][c] != '$' && temp[i][c] != ' ' && temp[i][c])
                c++;
            temp[i] = ft_strjoini(match_env(data, ft_substr(temp[i], 0, c)), temp[i], c);
            free(stemp);
        }
        i++;
    }
}