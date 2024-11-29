#include "kernel/types.h"
#include "user/user.h"

void primes(int leftp)
{
    int prime;
    if(read(leftp, &prime, sizeof(prime)) == 0)
    {
        close(leftp);
        return;
    }
    printf("prime %d\n", prime);

    int rightp[2];
    pipe(rightp);
    if(fork() == 0)
    {
        close(leftp);
        close(rightp[1]);
        primes(rightp[0]);
    }
    else
    {
        close(rightp[0]);
        int n;
        while(read(leftp, &n, sizeof(n)))
        {
            if (n % prime != 0)
                write(rightp[1], &n, sizeof(n));
        }
        close(leftp);
        close(rightp[1]);
        wait(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        close(p[1]);
        primes(p[0]);
    }
    else
    {
        close(p[0]);
        int i;
        for (i = 2; i <= 280; i++)
            write(p[1], &i, sizeof(i));
        close(p[1]);
        wait(0);
    }
    exit(0);
}

/*** Original code (why it doesn't work?) ******
int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int i;
    for (i = 2; i <= 280; i++)
        write(p[1], &i, sizeof(i));
    close(p[1]);
    primes(p[0]);
    exit(0);
}
*/