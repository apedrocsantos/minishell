/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anda-cun <anda-cun@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:05:45 by anda-cun          #+#    #+#             */
/*   Updated: 2023/10/05 10:02:55 by anda-cun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	do_pipes(t_command_list *cmd_lst, t_pipe *pipes)
{
	if (cmd_lst->next)
	{
		pipe(pipes->fd);
		pipes->open = 1;
		if (cmd_lst->out_fd == -1)
			cmd_lst->out_fd = pipes->fd[1];
	}
	if (pipes->next->open == 1)
		cmd_lst->in_fd = pipes->next->fd[0];
	return (0);
}

int	assign_fds(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	return (0);
}

int	check_fds(t_command_list *cmd_lst, t_pipe *pipes, int i)
{
	if ((cmd_lst->next || pipes->next->open))
		do_pipes(cmd_lst, pipes);
	while (cmd_lst->arg[i].token != NULL)
	{
		if (cmd_lst->arg[i].type == IN)
			if (open_file(&cmd_lst->in_fd, cmd_lst->arg[i].token,
					O_RDONLY | O_CLOEXEC, 0))
				return (print_file_error("minishell: ", cmd_lst->arg[i].token));
		if (cmd_lst->arg[i].type == OUT)
			if (open_file(&cmd_lst->out_fd, cmd_lst->arg[i].token,
					O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, 0664))
				return (print_file_error("minishell: ", cmd_lst->arg[i].token));
		if (cmd_lst->arg[i].type == APPEND)
			if (open_file(&cmd_lst->out_fd, cmd_lst->arg[i].token,
					O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0664))
				return (print_file_error("minishell: ", cmd_lst->arg[i].token));
		if (cmd_lst->arg[i].type == HEREDOC)
		{
			if (mini_heredoc(cmd_lst->arg[i].token))
				return (print_file_error("minishell: ", "heredoc"));
			cmd_lst->in_fd = open("heredoc_163465", O_RDONLY | O_CLOEXEC);
		}
		i++;
	}
	return (assign_fds(cmd_lst->in_fd, cmd_lst->out_fd));
}

void	add_pid(t_data *data, int pid, t_command_list *cmd_lst)
{
	t_pid	*temp;

	temp = malloc(sizeof(t_pid));
	temp->value = pid;
	if (!cmd_lst->next)
		temp->last = 1;
	else
		temp->last = 0;
	temp->next = data->pid;
	data->pid = temp;
}

void	free_pid(t_data *data)
{
	t_pid	*temp;

	while (data->pid->value != 0)
	{
		temp = data->pid;
		data->pid = data->pid->next;
		free(temp);
	}
	data->pid->value = 0;
}

int	execute_execve(t_data *data, t_command_list *cmd_lst, char **args)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: error forking\n", 2);
		return (-1);
	}
	if (pid != 0)
	{
		g_signal++;
		add_pid(data, pid, cmd_lst);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (execve(cmd_lst->exec_path, args, NULL) == -1)
		{
			if (data->pipes.open)
				close(data->pipes.fd[0]);
			revert_fds(cmd_lst);
			ft_putstr_fd(cmd_lst->arg->token, 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
	}
	return (0);
}

void	wait_for_execve(t_data *data, int *status)
{
	t_pid	*pid;

	pid = data->pid;
	while (pid->value != 0)
	{
		waitpid(pid->value, status, 0);
		if (pid->value == data->pid->value && pid->last)
			data->exit_status = WEXITSTATUS(*status);
		pid = pid->next;
		g_signal--;
	}
	free_pid(data);
}

int	check_cmd(t_data *data, t_command_list *cmd_lst, t_pipe *pipes)
{
	char	**arg_list;
	int		status;

	data->pipes.open = 0;
	data->pipes.next->open = 0;
	while (cmd_lst)
	{
		init_cmd_lst(cmd_lst);
		// expand(cmd_lst->arg);
		arg_list = get_arg_list(cmd_lst->arg);
		check_path(data, cmd_lst, arg_list);
		if (check_fds(cmd_lst, pipes, 0) != 0)
		{
			if (pipes->open)
				close(pipes->fd[1]);
			data->exit_status = 1;
		}
		else if (arg_list && !is_builtin(data, arg_list))
			execute_execve(data, cmd_lst, arg_list);
		revert_fds(cmd_lst);
		free_path(data->path);
		free_cmd(arg_list, cmd_lst);
		pipes = pipes->next;
		cmd_lst = cmd_lst->next;
	}
	wait_for_execve(data, &status);
	return (data->exit_status);
}
