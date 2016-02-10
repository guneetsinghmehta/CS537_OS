#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>
void errorFn()
{
	char error_message[30]="An error has occured\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
}

int main(int argc ,char *argv[])
{	
	int rc=fork();	
	if(rc==0)
	{
		char *myargv[4];
		myargv[0]=strdup("/bin/ls");
		myargv[1]=strdup("-l");
		myargv[2]=strdup("-a");
		myargv[3]=NULL;
		execv(myargv[0],myargv);
		printf("failed\n");
		exit(1);
	}
	else if(rc>0)
	{
		int cpid=(int)wait(NULL);
		printf("parent : cpid was %d\n",cpid);
	}
	else{printf("failure");}	
	return 0;
}
