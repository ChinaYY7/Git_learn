#include "apue.h"
#include <sys/wait.h>
#define FIFO_path "/home/mr_yy/Program/FIFO/fifo"
void Error_Exit(const char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}
//创建pipe
void Tell_Wait_pipe(int *fd, int num)
{
    if (pipe(fd) < 0)
        Error_Exit("Creat pipe error\n");
    while(num)
    {
        if (write(fd[1],"c",1) != 1)
            Error_Exit("Write pipe error\n");
        num--;
    }
}

//读pipe
void Wait_pipe(int *fd, int num)
{
    char temp;

    while(num)
    {
        if (read(fd[0], &temp, 1) != 1)
            Error_Exit("Read pipe error\n");
        num--;
    }
}

//写pipe
void Tell_pipe(int *fd, int num)
{
    while(num)
    {
        if (write(fd[1],"c",1) != 1)
            Error_Exit("Write pipe error\n");
        num--;
    }
}

//关闭pipe
void Close_Tell_Wait_pipe(int *fd)
{
    close(fd[0]);
    close(fd[1]);
}

int main(void)
{
    int FIFO_fd[3],pipe_fd[2];
    pid_t pid[2];
    char buffer[4096];
    int count;
    long int sum_count = 0;

    if((mkfifo(FIFO_path, FILE_MODE)) < 0 && (errno != EEXIST))
        Error_Exit("Can not creat FIFO\n");

    Tell_Wait_pipe(pipe_fd, 0);   

    if((pid[1] = fork()) < 0)
        Error_Exit("fork1 error\n");
    else if(pid[1] == 0)
    {
        if ((FIFO_fd[1] = open(FIFO_path, O_WRONLY)) == -1)
            Error_Exit("Open fifo error\n");
        while(1)
        {
            if((count = write(FIFO_fd[1], buffer, 4096)) == -1)
                Error_Exit("Write FIFO error\n");
            sum_count+=count;
            printf("sum_count = %ld \n", sum_count / 1024);
        }
        Tell_pipe(pipe_fd,1);
    }

    if((pid[2] = fork()) < 0)
        Error_Exit("fork1 error\n");
    else if(pid[2] == 0)
    {
        if ((FIFO_fd[0] = open(FIFO_path, O_RDONLY)) == -1)
            Error_Exit("Open fifo error\n");
        Wait_pipe(pipe_fd,1);
    }
    wait(NULL);
    return 0;
    
}