#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
int main()
{
	
		char *temp="/bin//uname.out";
		FILE *f1=fopen(temp,"w");
		printf("%d",f1);		
				char *temp2="/bin//uname.err";
		FILE *f2=fopen(temp2,"w");
		printf("%d",f2);	
	return 0;
}
