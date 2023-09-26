/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eportela <eportela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 16:47:26 by anda-cun          #+#    #+#             */
/*   Updated: 2023/09/26 12:16:33 by eportela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int create_executor(char *command)
// {
//     if (fork() == 0)
//     {
//         signal(SIGINT, sigint_handler);
//         exec(&command);
//         kill(getpid(), SIGSEGV);
//     }
//     return (0);
// }

void	init(t_data *data, char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			data->path = ft_split(&envp[i][5], ':');
	}
	data->env = get_env(envp);
	data->exit_status = 0;
	data->pipes.next = malloc(sizeof(t_pipe));
	data->pipes.next->next = &data->pipes;
	data->heredoc = 0;
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

int	main(int ac, char **av, char **envp)
{
	t_data data;
	char **splitter;
	char *changes;
	char * line;
	t_command_list *cmd_lst;

	if (ac != 1 || av[1])
		return (ft_putstr_fd("Error: Too many arguments\n", 1));
	init(&data, envp);
	// signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, sigint_handler);
	while (1)
	{
		line = readline("minishell$>");
		if (line != NULL && *line)
		{
			add_history(line);
			if (!token_error(&data, line) && !check_unclosed(&data, line))
			{
				if (!ft_strncmp(line, "exit", 4) && (!line[4]
						|| line[4] == ' '))
				{
					free(line);
					break ;
				}
				changes = treat_str(line, 0, 0, 0);
				splitter = ft_split(changes, 2);
				int c = 0;
				while (splitter[c])
					printf("aoidsnfoiadsnfioa: %s\n", splitter[c++]);
				expand_var(&data, splitter, 0 , 0);
				cmd_lst = malloc(sizeof(t_command_list));
				cmd_lst->arg = malloc(sizeof(t_arg) * (ft_strleni(splitter, 0)
						+ 1));
				parsing(cmd_lst, splitter, 0);
				//check_cmd(&data, cmd_lst, &data.pipes);
				free_all(cmd_lst, changes, splitter);
				c = 0;
				while (splitter[c])
					printf("aoidsnfoiadsnfioa: %s\n", splitter[c++]);
			}
		}
		free(line);
		fprintf(stderr, "EXIT STATUS: %d\n", data.exit_status);
	}
	free_data(&data);
	return (0);
}