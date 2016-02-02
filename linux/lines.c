#include<stdio.h>
#include<stdlib.h>

void main(int argc, char* argv)
{

FILE *fp;

//file opens
printf("%s\n",argv[1]);
fp = fopen("argv[1]","r");
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

printf("The number of lines = %d\n",lines);
//create an array to store individual line information
//
}
}
