#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main(int argc, char* argv[])
{

FILE *fp;

//file opens
//printf("%d\n",abs(atoi(argv[1])));
fp = fopen(argv[1],"r");
if (fp==NULL) perror ("Error opening file boss");
  else
  {
//get number of lines in file
size_t lines=0;
int ch;
while(!feof(fp))
{
  ch = fgetc(fp);
  if(ch == '\n')
  {
    lines++;
  }
}

//while(lines--){

char *temp=(char*)malloc(3*sizeof(char));
if(fgets(temp,3,fp)==NULL){
//printf("iter=%d",9-lines);
           fprintf(stderr,"fgets failed");
           exit(1);
          }

printf("%s",temp);
//}

//printf("The number of lines = %d\n",lines);
//create an array to store individual line information
//
}
}
