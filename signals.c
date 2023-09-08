#include "minishell.h"

void sigint_handler(int signal)
{
    printf("Signal KIller %d\n", signal);
    main();
}

int arm_signals()
{
    signal(SIGINT, sigint_handler);
    return (0);
}