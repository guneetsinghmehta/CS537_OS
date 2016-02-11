#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXLINELENGTH 128

void callErrorFn()
{	
	//all functions call this function to display error
	char error_message[30]="An error has occured\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
	exit(1);
}

void parse(char *input , char **command)
{
	/*
		Returns command parsed as an array of strings ending at NULL
	*/
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
	const char s2[2]="\n";
	token=strtok(command[i-1],s2);
	command[i-1]=strdup(token);
	command[i]=NULL;
	i=0;
}

void searchCommand(char **command,char **path)
{
	//searches for commands in the specified paths
	/*
	takes in a command array. Loops over all paths specified and looks for the command
	returns normally if command is present. 
	If command is not present in any line then it displays an Error
	*/
	int i=0;
	char *temp=malloc(2*MAXLINELENGTH);
	char *temp2=malloc(2*MAXLINELENGTH);
	//Searches for built in commands in paths
	while(path[i]!=NULL)
	{
		temp2=strdup(path[i]);
		temp=strcat(temp2,"/");
		temp=strcat(temp,&command[0][0]);
		struct stat *buf=malloc(sizeof(struct stat));

		if(stat(temp,buf)==0)
		{
			return;//success
		}
		i++;
	}
	//searches for 	exit ,cd ,pwd and path
	if(strcmp(&command[0][0],"exit")==0||strcmp(&command[0][0],"cd")==0||strcmp(&command[0][0],"pwd")==0||strcmp(&command[0][0],"path")==0)
	{
		return;
	}
	callErrorFn();
}

void executeCommand(char **command, char **path)
{
	/*
		Searches for command present in one of the search folders
		On finding the command in a folder makes a child process and executes the command with 			parameters
	*/
 	int i=0;
	char *temp=malloc(2*MAXLINELENGTH);
	char *temp2=malloc(2*MAXLINELENGTH);
	while(path[i]!=NULL)
	{
		temp2=strdup(path[i]);
		temp=strcat(temp2,"/");
		temp=strcat(temp,&command[0][0]);
		struct stat *buf=malloc(sizeof(struct stat));
		//Searches for built in commands in paths
		if(stat(temp,buf)==0)
		{
			int rc=fork();
			if(rc==0)
			{
				execv(temp,command);
				callErrorFn();				
			}
			else if(rc>0)
			{
				int cpid=(int)wait(NULL);
			}
			else
			{
				callErrorFn();	//if fork fails			
			}
			return;//success
		}
		i++;
	}

}
/*
	4 depending on tokens - 
	a. if in built then call the function
	b. if cd /exit/pwd /path then call branches in code - make functions in fact
	
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
	for(;;)
	{
		printf("whoosh> ");
		fgets(input,MAXLINELENGTH,stdin);
		parse(input,command);
		searchCommand(command,path);
		executeCommand(command,path);
	}
	return 0;
}


























