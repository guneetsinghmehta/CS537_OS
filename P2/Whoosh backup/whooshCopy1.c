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
#define PATH_MAX 128

void callErrorFn()
{	
//all functions call this function to display error
	char error_message[30]="An error has occured\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
}

void parse(char *input , char **command)
{
//Returns command parsed as an array of strings ending at NULL
	const char s[2]=" ";
	char *inputCopy=strdup(input);
	char *token;
	int i=0;
	token=strtok(inputCopy,s);
	while(token!=NULL)
	{
		command[i]=strdup(token);
		i++;
		token=strtok(NULL,s);
	}
	const char s2[2]="\n";
	token=strtok(command[i-1],s2);
	if(token!=NULL)
	{
		command[i-1]=strdup(token);	
		command[i]=NULL;
	}
	else{command[i-1]=NULL;}
}

int searchCommand(char **command,char **path)
{
	//searches for commands in the specified paths
	/*
	takes in a command array. Loops over all paths specified and looks for the command
	returns normally if command is present. 
	If command is not present in any line then it displays an Error
	*/
	int i=0;
	char *temp=malloc(2*MAXLINELENGTH);
	//Searches for built in commands in paths
	while(path[i]!=NULL)
	{
		temp=strcat(strcat(strdup(path[i]),"/"),&command[0][0]);
		struct stat *buf=malloc(sizeof(struct stat));
		if(stat(temp,buf)==0){return 0;}//success		
		i++;
	}
	//searches for 	exit ,cd ,pwd and path
	if(strcmp(&command[0][0],"exit")==0||strcmp(&command[0][0],"cd")==0||strcmp(&command[0][0],"pwd")==0||strcmp(&command[0][0],"path")==0){return 0;}
	return 1;
}

int numCommands(char **command)
{
	int ans=0;
	while(command[ans]!=NULL){ans++;}
	return ans;
}

int checkRedirection(char **command)
{	
	//checks if redirection needs to be done. If yes then returns 0 on success and 1 on fault
	//returns 0 on success and 1 on failure
	//incomplete - check for segmentation fault - 
	/*
		case 1 - ls >
		case 2 - ls > out out2 out3
		case 3 - ls > out
	*/
	int i=0,count=numCommands(command);
	while(command[i]!=NULL)
	{
		if(strcmp(command[i],">")==0&&i==count-2)
		{
			char *temp;
			close(STDOUT_FILENO);temp=strdup(command[i+1]);
			open(strcat(temp,".out"),O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR|S_IWUSR);
			close(STDERR_FILENO);temp=strdup(command[i+1]);
			open(strcat(temp,".err"),O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR|S_IWUSR);
			return 0;
		}
		else if(strcmp(command[i],">")==0&&(i==count-1||i<count-2))
		{
		 return 1;
		}
		i++;
	}
}

void executeCommand(char **command, char **path)
{
	/*
		Searches for command present in one of the search folders
		On finding the command in a folder makes a child process and executes the command with 			parameters
	*/
 	int i=0;int j;
	char *temp=malloc(2*MAXLINELENGTH);
	while(path[i]!=NULL)
	{
		temp=strcat(strcat(strdup(path[i]),"/"),&command[0][0]);
		struct stat *buf=malloc(sizeof(struct stat));
		//Searches for built in commands in paths
		if(stat(temp,buf)==0&&strcmp(command[0],"pwd")!=0)//because pwd is new command
		{
			j=checkRedirection(command);
			printf("kipper");

			int rc=fork();
			if(rc==0)
			{
				if(checkRedirection(command)==0)
				{
					execv(temp,command);
					callErrorFn();
					exit(0);
				}
				else{callErrorFn();return; }
			}
			else if(rc>0){int cpid=(int)wait(NULL);}
			else {callErrorFn();}	//if fork fails		
			return;//success
		}
		i++;
	}
	if(strcmp(&command[0][0],"exit")==0){exit(0);return;}
	if(strcmp(&command[0][0],"path")==0)
	{
	//incomplete
		printf("**my implementation**\n");
		int i=1;
		**path=NULL;
		while(command[i]!=NULL)
		{
			path[i-1]=strdup(command[i]);
			printf("%s\n",path[i-1]);
			i++;
		}
		return;
	}
	
	if(strcmp(&command[0][0],"pwd")==0)
	{	
		char buff[PATH_MAX+1];
		char *cwd=getcwd(buff,PATH_MAX+1);
		printf("%s\n",cwd);
		return;
	}
	
	if(strcmp(&command[0][0],"cd")==0)
	{	
		char buff[PATH_MAX+1];
		if(command[1]==NULL)
		{
			char *cwd=strdup(getenv("HOME"));
			if(chdir(cwd)==-1){callErrorFn();}
		}
		else if(command[2]==NULL)
		{	
			char *cwd=strcat(strcat(getcwd(buff,PATH_MAX+1),"/"),command[1]);
			if(chdir(cwd)==-1){callErrorFn();}
		}
		else{callErrorFn();exit(1);}//when more than one argument for cd
		return;
	}
	

}
void main(int argc ,char *argv[])
{	
	char input[10*MAXLINELENGTH];
	char **command;	command=malloc(MAXLINELENGTH);// will contain ls -la /tmp etc etc ...
	char **path;path=(char**)malloc(MAXLINELENGTH);
	path[0]=strdup("/bin");
	for(;;)
	{
		printf("whoosh> ");
		fgets(input,MAXLINELENGTH,stdin);
		if(strlen(input)==1){continue;}
		if(strlen(input)>128){callErrorFn();continue;}
		parse(input,command);
		if(searchCommand(command,path)==1){callErrorFn();continue;}//0 if success and 1 if failure
		executeCommand(command,path);
	}
}


























