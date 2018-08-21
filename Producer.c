//gcc Producer.c -o Producer
//./Producer &
#include "apue.h"
#define FIFO_path "/home/mr_yy/Program/FIFO/fifo4"   
#define BUFFER_SIZE PIPE_BUF  
#define TEN_MEG (1024 * 1024 * 50)  
int main(void)  
{  
    int pipe_fd;  
    int res;  
    int open_mode = O_WRONLY;  
    long int bytes = 0;  
    char buffer[BUFFER_SIZE + 1];  

    if (access(FIFO_path, F_OK) == -1)  
    {  
        res = mkfifo(FIFO_path, FILE_MODE);  
        if (res != 0)  
        {  
            fprintf(stderr, "Could not create fifo %s\n", FIFO_path);  
            exit(EXIT_FAILURE);  
        }  
    }  
    printf("\nProcess %d opening FIFO O_WRONLY\nWaiting customer.....\n", getpid());  
    pipe_fd = open(FIFO_path, open_mode);  
    if (pipe_fd != -1)  
    {  
        printf("Process %d result %d\n", getpid(), pipe_fd); 
        printf("Process %d write in .....\n", getpid()); 
        while (bytes < TEN_MEG)  
        {  
            res = write(pipe_fd, buffer, BUFFER_SIZE);  
            if (res == -1)  
            {  
                fprintf(stderr, "Write error on pipe\n");  
                exit(EXIT_FAILURE);  
            }  
            bytes += res;  
        }  
        close(pipe_fd);  
    }  
    else  
    {  
        exit(EXIT_FAILURE);  
    }  
    printf("Process %d finish writing, %ld bytes write\n", getpid(), bytes);
    exit(EXIT_SUCCESS);  
}  
