#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

//#define MB (0+1048576)
int mb_to_bytes(int );

int main (int argc , char *argv[])
{
	printf("1 Mb is %d bytes \n",mb_to_bytes(1));
	printf("1 Mb is %d bytes \n",mb_to_bytes(2));
	return 0;
}
