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
	printf("%d\n", g_signal);
	if (signal && g_signal == 0)
		terminal_prompt();
	else if (signal)
		printf("\n");
}
