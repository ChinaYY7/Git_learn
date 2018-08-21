#include "apue.h"
int main()  
{  
    
    if (mkfifo("/home/mr_yy/Program/FIFO/fifo2", 0755) != 0)  
        perror("creat FIFO error!");//PC_PIPE_BUF = 4096
    else
        printf("FIFO created\n");    
    exit(0);  
}  