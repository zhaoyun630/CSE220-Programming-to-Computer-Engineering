#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc,char* argv[])
{
	int a1 = 0;
	int a2 = 0;
	int a3[32] = { 0 };
	int a4 = atoi(argv[1]);
	long a5 = a4 > 0?a4:(pow(2,32)+a4);
	while(a5 > 0)
        {
		a3[31 - a2] = a5 % 2;
                a2++;
                a5 = a5 / 2;
        }

	printf("The binary representation for integer %d is: ",a4);
	
	for(a1=32-a2;a1<32;a1++)
	{
		printf("%d",a3[a1]);
		if(((a1+1) % 4) == 0)printf(" ");
	}
	
	printf("\n");
	return 0;
}


