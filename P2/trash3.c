#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>
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
	if(token!=NULL)
	{
		command[i-1]=strdup(token);	
		command[i]=NULL;
	}
	else{command[i-1]=NULL;}
	i=0;
}

int main(int argc ,char *argv[])
{
	//correct use of pointers
	char temp[128];
	fgets(temp,sizeof(temp),stdin);
	char **command;
	parse(temp,command);
	int i=0;
	while(command[i]!=NULL)
	{
			printf("%s\n",command[i]);
			i++;		
	}

	return 0;
}
