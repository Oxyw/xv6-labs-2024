#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXBUF 512

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    char *args[MAXARG];
    int i, argcnt = argc - 1;
    for (i = 0; i < argcnt; i++)
        args[i] = argv[i + 1];
    args[argcnt] = malloc(MAXBUF);
    args[argc] = 0;
    while (gets(args[argcnt], MAXBUF))
    {
        if (args[argcnt][0] == '\0')
            break;
        if (args[argcnt][strlen(args[argcnt]) - 1] == '\n') // gets() will preserve '\n' at the end of the line
            args[argcnt][strlen(args[argcnt]) - 1] = '\0';
        if (fork() == 0)
        {
            if(exec(args[0], args) == -1)
            {
                fprintf(2, "xargs: exec %s failed\n", args[0]);
                exit(1);
            }
        }
        else
            wait(0);
    }
    exit(0);
}