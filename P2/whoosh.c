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

void parse(char *input , char **command)
{
	const char s[2]=" ";
	char *inputCopy=strdup(input);
	char *token;
	int i=0;
	token=strtok(inputCopy,s);
	while(token!=NULL)
	{
		//printf("%s ",token);
		command[i]=strdup(token);
		i++;
		token=strtok(NULL,s);
	}
}

void searchCommand(char *input)
{
	//searches for commands in the specified paths
}
/*
	3 split into tokens
	4 depending on tokens - 
	a. if in built then call the function
	b. if cd /exit/pwd /path then call branches in code - make functions in fact
	
	1. parse into chars
	2. search command
	3. execute command
*/

int main(int argc ,char *argv[])
{	
	char input[128];
	char **command;	command=malloc(128);// will contain ls -la /tmp etc etc ...

	printf("whoosh> ");
	fgets(input,128,stdin);
	parse(input,command);
		
	return 0;

}


























