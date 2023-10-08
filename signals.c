#include "includes/minishell.h"

void	terminal_prompt(void)
{
	rl_reset_line_state();
	rl_cleanup_after_signal();
	rl_replace_line("", 0);
	rl_crlf();
	rl_redisplay();
}

void	sigint_handler(int signal)
{
	if (signal && g_signal == 0)
		terminal_prompt();
	else if (signal)
		printf("\n");
}

void heredoc_sigint_handler(int signal)
{
	if (signal && !g_signal)
		exit(1);
	// if (signal && g_signal == 0)
	// 	printf("\n");
}
