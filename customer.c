//gcc customer.c -o customer
//time ./customer
#include "apue.h"
#define FIFO_path "/home/mr_yy/Program/FIFO/fifo4"  
#define BUFFER_SIZE PIPE_BUF//4096   
int main(void)  
{  
    int pipe_fd;  
    int res;  
    int open_mode = O_RDONLY;  
    char buffer[BUFFER_SIZE + 1];  
    long int bytes = 0;  
    memset(buffer, '\0', sizeof(buffer));//数组清空
    printf("\nProcess %d opeining FIFO O_RDONLY\n", getpid());  
    if ((pipe_fd = open(FIFO_path, open_mode)) == -1)  
    {  
        perror("Could not open fifo"); 
        exit(EXIT_FAILURE);
    }
    printf("Process %d result %d\n", getpid(), pipe_fd); 
    printf("Process %d read  out .....\n", getpid()); 
    do
    {  
        res = read(pipe_fd, buffer, BUFFER_SIZE);  
        bytes += res;  
    }
    while(res > 0);  
    close(pipe_fd);
    
    printf("Process %d finished reading, %ld bytes read\n", getpid(), bytes);  
    exit(EXIT_SUCCESS);  
}  