//避免竞争条件8-12
//gcc 8-12.c TELL_WAIT_PIPE/TELL_WAIT_PIPE.c -o ../bin/8-12.out -I TELL_WAIT_PIPE -I .
#include "apue.h"
#include "TELL_WAIT_PIPE.h"
static void charatatime(char *str)
{
    char *ptr;
    int c;
    setbuf(stdout,NULL);
    for(ptr = str; (c = *ptr++) != 0;)
        putc(c,stdout);
}
int main(void)
{
    pid_t pid;
    TELL_WAIT();
    if((pid = fork()) < 0)
        perror("fork error!\n");
    else if (pid == 0)
    {
        Wait_parent();
        charatatime("output from child \n");
        Tell_parent(pid);
        
    }
    else
    {
        charatatime("output from parent \n");
        Tell_child(pid);
        Wait_child();
    }
    exit(0);
}
