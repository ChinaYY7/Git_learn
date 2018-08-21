//fork函数实例8-1
#include "apue.h"
int global = 6;
char buf[] = "a write to stdout\n";
int main(void)
{
    int var = 88 ;
    pid_t pid;
    /*
    sizeof计算包括终止null字节，不需要输出，因此减一，strlen计算则不包括终止null字节
    sizeof编译时计算缓冲区长度，strlen是函数调用
    */
    if(write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)//write 不带缓冲，立即输出
        perror("write error\n");
    /*
    printf为标准IO库函数，带缓冲，如果标准输出连到终端，则是行缓冲，遇到换行符输出（fflush）
    若标准输出连到磁盘上文件则为全缓冲，进程结束时或写满缓冲输出（fflush）
    */
    printf("before fork\n");
    if((pid = fork()) < 0)
        perror("fork error\n");
    else if(pid == 0)
    {
    /*
    标准IO库函数分配的缓冲在进程的堆区，若是全缓冲时，printf的数据留在堆区分配的缓冲区中，且会被子进程拷贝，子进程所有调用的printf都会把
    数据写入缓冲区，直到子进程退出时才会输出，父进程也是。
    若时行缓冲，遇到'\n'时就会把缓冲区数据输出，子进程就不会拷贝到
    */
        global++;
        var++;
        printf("child:");
    }
    else
    {
        sleep(2);
        printf("parent:");
    }
        
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), global, var);
    exit(0);
}