#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>
void callErrorFn()
{	
	char error_message[30]="An error has occured\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
}
/*
	1 read in char 128
	2 make infintie loop 
	3 split into tokens
	4 depending on tokens - 
	a. if in built then call the function
	b. if cd /exit/pwd /path then call branches in code - make functions in fact
	5. 
*/

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
		char *temp=strdup(myargv[0]);
		execv(temp,myargv);
		callErrorFn();
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
