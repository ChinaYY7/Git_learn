#include <stdio.h>
int main(void)
{
	int i;
	int tol;
	tol = 0;
	for(i = 0; i <= 10; i++)
		tol += i;
	printf("1+2+....+10 = %d\n",tol);
	return 0;
}
