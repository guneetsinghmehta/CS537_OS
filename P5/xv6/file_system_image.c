#include<stdio.h>
int main(int argc, char *argv[])
{
	int *f1=(int*)fopen(argv[1],"r");
	if(f1==0)
	{fprintf(stderr,"image not found");return 1; }
	
	
	
	return 0;
}
