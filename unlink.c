//打开一个文件，然后unlink它4-16
#include "apue.h"
#include <fcntl.h>
int main(void)
{
	if(open("/home/mr_yy/Program/doc/file.hole",O_RDWR) < 0)
		perror("open error");
	if(unlink("/home/mr_yy/Program/doc/file.hole")<0)
		perror("unlink error");
	printf("file unlinked\n");
	sleep(15);
	printf("done\n");
	exit(0);
}
