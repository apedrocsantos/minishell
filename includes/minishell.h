#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

extern int					g_signal;

enum						tokens
{
	STR,     // str
	IN,      // <
	OUT,     // >
	APPEND,  // >>
	HEREDOC, // <<
	PIPE,    // |
	END,     // ; (end of command)
};

typedef struct s_node
{
	char					*content;
	struct s_node			*next;
}							t_node;

typedef struct s_pair
{
	char					*key;
	char					*value;
	struct s_pair			*next;
}							t_pair;

typedef struct s_pipe
{
	int						fd[2];
	int						open;
	struct s_pipe			*next;
}							t_pipe;

typedef struct s_pid
{
	int						value;
	int						last;
	struct s_pid			*next;
}							t_pid;

typedef struct s_data
{
	char					*name;
	int						heredoc;
	int						exit_status;
	int						exit;
	t_pid					*pid;
	t_pair					*env;
	t_pair					*exported_vars;
	char					**path;
	struct s_pipe			pipes;
	char					cwd[PATH_MAX];
}							t_data;

typedef struct s_arg
{
	char					*token;
	int						type;
}							t_arg;

typedef struct s_command_list
{
	char					*exec_path;
	t_arg					*arg;
	int						stdin;
	int						stdout;
	int						in_fd;
	int						out_fd;
	struct s_command_list	*next;
}							t_command_list;

/**BUILTINS*/
int							cd(t_data *data, char **str);
int							echo(t_data *data, char **str);
int							env(t_data *data, char **str);
int							export(t_data *data, char **str);
int							pwd(t_data *data, char **str);
int							unset(t_data *data, char **str);
int							exit_builtin(t_data *data, char **str);
int							is_builtin(t_data *data, char **args);

void						print_struct(t_command_list *lst);
int							token(char *line);
int							ft_strleni(char **splitter, int i);
int							z_cmp(char *str, char *cmp);
void						free_all(t_command_list *cmd_lst, char *changes,
								char **splitter);
void						parsing(t_command_list *cmd_lst, char **splitter,
								int i);
int							create_executor(char *command);
void						sigint_handler(int signal);
int							exec(char **str);
int							expand_path(char *str);
t_pair						*get_env(char **envp);
void						cleanup(t_data *data);
void						free_nodes(t_node *node);
void						print_pairs(t_pair *pair);
void						print_exported(t_node *node);
void						free_pairs(t_pair *pair);
void						sort_list(t_pair *export);
int							print_syntax_error(char c);
int							print_file_error(char *s1, char *s2);
int							check_parse_errors(t_data *data, char *str);
void						terminal_prompt(void);
int							print_parse_error(char c);
char						*treat_str(char *line, char aspas, int j);
int							mini_heredoc(t_data *data, char *str);
int							check_cmd(t_data *data, t_command_list *cmd_lst,
								t_pipe *pipes);
char						*get_next_line(int fd);
int							token_error(t_data *data, char *str, int *i);
char						**get_arg_list(t_arg *arg);
int							open_file(int *fd, char *filename, int flags,
								int perms);
void						revert_fds(t_command_list *cmd_lst);
int							check_path(t_data *data, t_command_list *cmd_lst, char **str);
void						free_data(t_data *data);
void						free_args(t_arg *arg);
void						free_cmd(char **arg_list, t_command_list *cmd_lst,
								int *heredoc);
void						init_cmd_lst(t_command_list *cmd_lst);
int							check_fds(t_data *data, t_command_list *cmd_lst,
								t_pipe *pipes, int i);
size_t						len_to_char(char *str, char c);
void						free_path(char **path);
void						sigintchild_handler(int singal);
void						expand(t_arg *arg);
int							replace_var(t_pair *env, char *str);
int						add_to_list(char *str, t_pair *exported_vars);
int							check_dupes(t_pair *env, char *str);
int							print_export_error(t_data *data, char *str);

#endif