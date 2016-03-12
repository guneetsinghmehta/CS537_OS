//Anmol Mohanty
//2/16/16
//whoosh.c
//WhooSH shell interpreter
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "limits.h"

#define BUF_SIZE 128

char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

//------GLOBAL STUFF-------//
//global error message


void print_error(){
char error_message[30] = "An error has occurred\n";
write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(int argc, char* argv[])
{
    if(argc>1){//bad argument
    print_error();
    //puts("wrong arg count");
    exit(1);
    }
    else{

//char prompt_message[9] = "whoosh> ";

char *com_path[130]; //no init?
int path_index=1; //index of all paths, default /bi - hence =1n
com_path[0]="/bin"; //default path /bin

struct stat sb;//command finder

int redir_count=0;
int redir_loc=0;
int redir_path_valid=0;
int redir_valid=0;
char *ptr,buff[PATH_MAX+1];
char *redir_path;
char *myargv[130]; 
int i=0,j=0,rc=0;

while(1){
    redir_valid=0;
        char* prompt_message="whoosh> ";
        printf("%s",prompt_message);
        //write(STDOUT_FILENO, prompt_message, strlen(prompt_message));
        fflush(stdout);
        char command[BUF_SIZE];
        redir_count=0; 
        
        if(fgets(command, 1300, stdin)!=NULL)
        {
           int c_len=strlen(command); //commands length
           if(c_len>129){//too long
               print_error();//puts("long");
               //while(fgets(stdin));
               continue; //loop back to prompt
           }else{
               //parse the command - pre processing step
               char com_parse[129][129];// Max of 128 args with chars max in each
               int com_index=0;
               char *token=strtok(command," ");
               while(token!=NULL){
                   memcpy(com_parse[com_index],token,128);
                   com_index++;
                   token=strtok(NULL," ");
               }
               //ignore argument ending in return carriage
               if(strcmp(com_parse[com_index-1],"\n")==0){
                   com_index--;
               }

               //removing the retyrn carriage in last arg string
               int last_com_len=strlen(com_parse[com_index-1]);
               if(com_parse[com_index-1][last_com_len-1]== '\n'){
               com_parse[com_index-1][last_com_len-1]= '\0'; //make it NULL char instead
               }
            
               for(i=0;i<com_index;i++){
                if(strcmp(com_parse[i],">")==0){
                    //printf("com_parse[%d]=%s\n",i,com_parse[i]);
                    redir_count++;
                    redir_loc=i;
                }
            }
            //printf("rdir count=%d",redir_count);
            if(redir_count==1){ //valid redir count
                //printf("rdir_count=%d rdir_loc=%d,com_index=%d\n",redir_count,redir_loc,com_index);
                if(redir_loc!=(com_index-2)){ //investigate, invalid loc, can be redired to one file only
                    print_error(); //puts("invalid place of redir");
                    continue;
            }
             
            redir_valid=1; 
            if(com_parse[com_index-1][0]=='/') { //check if path in last store
            ptr=getcwd(buff,PATH_MAX+1);//cwd, store it to get back after testing path validity
            int path_valid=chdir(com_parse[com_index-1]);
            if(path_valid==0){
              redir_path_valid=1;
              redir_path=strdup(com_parse[com_index-1]);
              chdir(ptr); //come back to current dir
            }
            else{
                chdir(ptr);
                print_error();//puts("invalid path to chdir");
                continue;
                }
            }

            com_index=com_index-2; //erase command -- investigate,keeping the core command now
           }

           else if(redir_count>1){ //invalid count of redirects
               print_error();//puts("invalid redirs");
               continue;
           }



           //**************************8
//redirection determination done
    if(strcmp(com_parse[0],"cd")==0 && com_index ==1){ //cd no args
                int cd_valid=chdir(getenv("HOME"));
                if(cd_valid!=0){//change failed
                    print_error(); //puts("cd error");
                    continue;
                }
    } else if(strcmp(com_parse[0],"cd")==0 && com_index ==2){ //cd no args
                int cd_valid=chdir(com_parse[1]);
                if(cd_valid!=0){//change failed
                    print_error();//puts("cd2 error");
                }
    }
            //second command-PWD
            else if(strcmp(com_parse[0],"pwd")==0)
            {
                ptr=getcwd(buff,PATH_MAX+1);
                if(ptr!=NULL){
                    printf("%s\n",ptr);
                }else{
                    print_error();//puts("pwd error");
                }
            }
            else if(strcmp(com_parse[0],"exit")==0){
            exit(0);    
            } else if( strcmp(com_parse[0], "\n") == 0) {// null argument
              continue;
            }
            //4th command - PATH updation logic for path check
            else if(strcmp(com_parse[0],"path")==0 && com_index==1){//path no args
           com_path[0]=NULL; //nothing in path 
            path_index=0;
            } 
    
            else if(strcmp(com_parse[0],"path")==0 && com_index>1){//path with >1 args
                path_index=0;
            for(i=1;i<com_index;i++){
                if(com_parse[i][0]=='/'){//if legal, update new path
                    com_path[i-1]=strdup(com_parse[i]);
                    path_index++;
                }
                else{//bad path
                    print_error(); //puts("bad path");
                }
                }
            }

            //end of built-in commands
            else //execute the other command via execv
            {
                 
            int com_valid=0;
            ptr = getcwd(buff, PATH_MAX + 1); // keep current directory
           // printf("%s",com_path[0]);
            for(i = 0; i < path_index; i++) {  // check every path to find command
                //printf("%s",com_path[0]);
              chdir(com_path[i]);              
              int com_st = stat(com_parse[0], &sb); //investigate
              if(com_st==0){//found command in paths, prepare to execute
                 // //puts("found ls\n");
                  com_valid=1;
                 //printf("comv=%d",com_valid);
                  char *s1=concat(com_path[i],"/"); //appends the "/" at end
                  char *s2=concat(s1,com_parse[0]);//merging commans and path
                myargv[0]=s2;//merge path and command together
               for(j=1;j<com_index;j++)
               {
                   myargv[j]=com_parse[j];
               }

               myargv[com_index]=NULL;
               chdir(ptr); //return to cd
               break;
              }
            }
                
            if(com_valid == 0) {  // if not found
                ////puts("why entering here");
              chdir(ptr);   //return back to current directory and 
              print_error(); //puts("invalid command");//print error 
              continue; //next command
            } else {  //  if found, then execute
               rc=fork(); //create a new process
               if(rc==0){//child spawned, execute command
                   ////puts("child spawned");
                       if(redir_valid==1){
                           if(redir_path_valid==1){
                               chdir(redir_path);
                           }
                           close(1);

                  open("output.out", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
                  close(2);
                  open("output.err", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
                       }
                       printf("executing %s",myargv[0]);
                   //puts("child spawned 2");
                       execv(myargv[0],myargv);
                       print_error(); //shouldn't return from prev if succes
                       exit(1);
                   }else if(rc>0) {//parent=>wait for child
                       wait(NULL);
                   }
               }
            }
   
       } 
   
        }        
   else{//fgets loop, err then return NULL
       print_error();//puts("data not inputted");
       continue;
   
   }

   }

}
return 0;
}

                
                
                
                
                
                
                
