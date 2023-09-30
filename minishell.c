/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:47:26 by anda-cun          #+#    #+#             */
/*   Updated: 2023/09/30 17:42:43 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal;

void	init(t_data *data, char **envp)
{
	data->env = get_env(envp);
	data->exit_status = 0;
	data->exit = 0;
	data->exported_vars = malloc(sizeof(t_pair));
	data->exported_vars->key = NULL;
	data->exported_vars->value = NULL;
	data->exported_vars->next = NULL;
	data->pipes.open = 0;
	data->pipes.next = malloc(sizeof(t_pipe));
	data->pipes.next->next = &data->pipes;
	data->heredoc = 0;
	data->path = NULL;
	data->pid = malloc(sizeof(t_pid));
	data->pid->value = 0;
	data->pid->next = NULL;
	getcwd(data->cwd, PATH_MAX);
}

void	init_cmd_lst(t_command_list *cmd_lst)
{
	cmd_lst->exec_path = NULL;
	cmd_lst->in_fd = -1;
	cmd_lst->out_fd = -1;
	cmd_lst->stdin = dup(STDIN_FILENO);
	cmd_lst->stdout = dup(STDOUT_FILENO);
}

void	minishell(t_data *data, char *line)
{
	char			**splitter;
	char			*changes;
	t_command_list	*cmd_lst;

	while (!data->exit)
	{
		if (!line)
			line = readline("minishell$> ");
		if (!line)
			exit_builtin(data, NULL);
		else if (line)
		{
			add_history(line);
			if (!token_error(data, line) && !check_unclosed(data, line))
			{
				changes = treat_str(line, 0, 0);
				splitter = ft_split(changes, 2);
				cmd_lst = malloc(sizeof(t_command_list));
				cmd_lst->arg = malloc(sizeof(t_arg) * (ft_strleni(splitter, 0)
						+ 1));
				parsing(cmd_lst, splitter, 0);
				check_cmd(data, cmd_lst, &data->pipes);
				free_all(cmd_lst, changes, splitter);
			}
		}
		free(line);
		line = NULL;
	}
}
int	main(int ac, char **av, char **envp)
{
	char *line;
	t_data data;

	line = NULL;
	g_signal = 0;
	if (ac == 3 && !ft_strncmp(av[1], "-c", 3))
		line = ft_strdup(av[2]);
	else if (ac != 1)
		return (ft_putstr_fd("Error: Wrong arguments\n", 1));
	init(&data, envp);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
	minishell(&data, line);
	free_data(&data);
}