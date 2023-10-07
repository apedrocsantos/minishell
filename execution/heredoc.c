/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:38:59 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/07 12:09:01 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	mini_heredoc(char *eof, t_command_list *cmd_lst)
{
	char	*out;
	char	*heredoc;
	char	*eofile;
	int		fd;
	int		pid;

	pid = fork();
	if (pid == -1)
		return(ft_putendl_fd("Error forking", 2));
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		heredoc = ft_strdup("");
		eofile = ft_strjoin(eof, "\0");
		fd = open("heredoc_163465", O_CREAT | O_WRONLY | O_TRUNC, 0664);
		if (fd == -1)
			return (1);
		while (ft_strncmp(heredoc, eofile, ft_strlen(eofile) + 1))
		{
			free(heredoc);
			heredoc = readline("> ");
			if (!heredoc)
				break ;
			out = ft_strjoin(heredoc, "\n");
			if (heredoc && ft_strncmp(heredoc, eofile, ft_strlen(eofile) + 1))
				write(fd, out, ft_strlen(out));
			free(out);
		}
		free(heredoc);
		free(eofile);
		close(fd);
		revert_fds(cmd_lst);
		exit(0);
	}
	wait(NULL);
	revert_fds(cmd_lst);
	return (0);
}

// int main(int ac, char **av)
// {
//     if (ac < 2)
//         return (1);
//     mini_heredoc(av[1]);
// }
