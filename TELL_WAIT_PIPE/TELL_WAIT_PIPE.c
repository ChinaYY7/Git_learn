//（无名）pipe管道实现避免竞争条件
#include "apue.h"
#include "TELL_WAIT_PIPE.h"
#define Child_read pfd1[0]
#define Parent_write pfd1[1]
#define Parent_read pfd2[0]
#define Child_write pfd2[1]
static int pfd1[2],pfd2[2];
void TELL_WAIT(void)
{
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
        perror("pipe error!\n");
}

void Tell_parent(pid_t pid)
{
    if (write(Child_write,"c",1) != 1)
        perror("write error!\n");
}
void Wait_child(void)
{
    char chr;
    if (read(Parent_read, &chr, 1) != 1)
        perror("read error!\n");
    if (chr != 'c')
        perror("Wait_child: incorrect data!\n");
    else
        printf("Child done !\n");
}

void Tell_child(pid_t pid)
{
    if (write(Parent_write,"p",1) != 1)
        perror("write error!\n");
}
void Wait_parent(void)
{
    char chr;
    if (read(Child_read, &chr, 1) != 1)
        perror("read error!\n");
    if (chr != 'p')
        perror("Wait_parent: incorrect data!\n");
    else
        printf("Parent done !\n");
}
