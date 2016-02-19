//Anmol Mohanty
//2/16/16
//whoosh.c
//WhooSH shell interpreter
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
  
#define COM_SIZE_MAX 128
#define BUF_SIZE 512

//global error message

char error_message[30] = "An error has occurred\n";
char prompt_message[8] = "whoosh> ";
//check init
char **path=NULL;
int main(int argc, char* argv[])
{
    

//command line looper
//
//while or for loop for making things shorter
//


    while(1)
    {

    write(STDOUT_FILENO, prompt_message,strlen(prompt_message));
    FILE *file = NULL;
    char buffer[BUF_SIZE];


  //  //input params
  //  if(argc==1){
  //      file=stdin;
  //      write(STDOUT_FILENO, prompt_message,strlen(prompt_message));
  //  }
//Handling built in commands
char *token = strtok(buffer, " \t\n");
    //cd to $HOME :: first built in command
    if(strcmp(token,"cd")==0)
            {
                token=strtok(NULL," \t\n");
                if(token=NULL)//go to home{
                {
                    chdir(getenv("HOME"));
                }
                else {
                    int chsucs=chdir(token);//0 for success
                    if(chsucs!=0)
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                }
            }
            //second command-PWD
            else if(strcmp(token,"pwd")==0)
            {
                if(strtok(NULL," \n\t")!=NULL){
                        write(STDERR_FILENO, error_message, strlen(error_message));
                }
                else{
                    char pwd[BUF_SIZE];
                    char *directory=getcwd(pwd,BUF_SIZE);
                    write(STDOUT_FILENO, directory, strlen(directory)); //outputs the dir
                    write(STDOUT_FILENO, "\n", 1);//newline character, checkout syntax of write api
                }
            }
            //third command - exit
            else if(strcmp(token,"exit")==0){
                if(strtok(NULL," \n\t")!=NULL){
                        write(STDERR_FILENO, error_message, strlen(error_message));
                }
                else
                    exit(0);
            }
            //4th command - PATH updation logic for path
            else if(strcmp(token,"path")==0)
            {
           //add to the list of path structure
            
            token=strtok(NULL," ");
            if(token!=NULL){
            do{
                
            path[counter++]=token;
            token=strtok(NULL," ");
            }while(token!=NULL);
            }

}

