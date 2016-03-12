#include <mylib.h>

char error_message[30] = "An error has occurred\n";

char prompt_message[8] = "whoosh> ";
int main(int argc, char* argv[]){ 
                if(strcmp(token,"cd")==0) {
                token=strtok(NULL," \t\n");
                if(token=NULL)chdir(getenv("HOME"));
                else { int chsucs=chdir(token);//0 for success
                    if(chsucs!=0) write(STDERR_FILENO, error_message, strlen(error_message));}}
            else if(strcmp(token,"pwd")==0){
                if(strtok(NULL," \n\t")!=NULL) write(STDERR_FILENO, error_message, strlen(error_message));
                else{ char pwd[BUF_SIZE];
                      char *directory=getcwd(pwd,BUF_SIZE);
                      write(STDOUT_FILENO, directory, strlen(directory)); //outputs the dir
                      write(STDOUT_FILENO, "\n", 1);}//newline character, checkout syntax of write api
            }
            //third command - exit
            else if(strcmp(token,"exit")==0){
                if(strtok(NULL," \n\t")!=NULL){
                        write(STDERR_FILENO, error_message, strlen(error_message));
                }
                else
                    exit(0);
            }
            //4th command - PATH
            else if(strcmp(token,"path")==0)
            {
            
            
            }

}

