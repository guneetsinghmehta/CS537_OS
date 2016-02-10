#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>
#define MAXLINELENGTH 128

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

void searchCommand(char *command,char **path)
{
	//searches for commands in the specified paths
	int i=0;
	//FILE *f1;
	char *temp=malloc(2*MAXLINELENGTH);
	while(path[i]!=NULL)
	{
		temp=strcat(path[i],"/");
		temp=strcat(temp,&command[0]);
		printf(temp);
		i++;
	}
}
/*
	4 depending on tokens - 
	a. if in built then call the function
	b. if cd /exit/pwd /path then call branches in code - make functions in fact
	
	2. search command
	3. execute command
*/

int main(int argc ,char *argv[])
{	
	char input[MAXLINELENGTH];
	char **command;	command=malloc(MAXLINELENGTH);// will contain ls -la /tmp etc etc ...
	char **path;path=malloc(MAXLINELENGTH);
	path[0]=strdup("/bin");
	path[1]=strdup("/usr/bin");
	
	//loop starts
	printf("whoosh> ");
	fgets(input,MAXLINELENGTH,stdin);
	parse(input,command);
	searchCommand(command[0],path);
	return 0;

}


























