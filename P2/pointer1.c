#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>
int main(int argc ,char *argv[])
{
	//correct use of pointers
	int i;
	char temp[128];
	for(i=0;i<5;i++)
	{
		scanf("%s",temp);
		argv[i]=strdup(temp);
		printf("%s\n",argv[i]);
		//printf("%s\n",temp);
	}
	return 0;
}
