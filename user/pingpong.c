#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buf[4] = {"abcd"};
    int p1[2], p2[2]; // pipe: read from p[0], write to p[1]
    pipe(p1); // parent to child
    pipe(p2); // child to parent

    if(fork() == 0)
    {
        close(p1[1]);
        close(p2[0]);
        read(p1[0], buf, 1);
        close(p1[0]);
        printf("%d: received ping\n", getpid());
        write(p2[1], buf, 1);
        close(p2[1]);
    }
    else
    {
        close(p1[0]);
        close(p2[1]);
        write(p1[1], buf, 1);
        close(p1[1]);
        wait(0);
        read(p2[0], buf, 1);
        close(p2[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}