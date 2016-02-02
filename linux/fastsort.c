/* Anmol Mohanty
 * P1 - Fast sort
 * All rights reserved
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main(int argc, char* argv)
{

//arguments check logic

//invalid number of arguments //throw error
if(argc<0 && argc >2)
{
//error message
fprintf(stderr, “Error: Bad command line parameters”);
//clean exit
exit(1);
}

else
{


if(argc==1)
compare_index=0;
else
//take care of the hyphen
compare_index=argv[1];

FILE *fp;

//file opens
fp = fopen("argv[2]","r");

//get number of lines in file

int count_lines=0;
//iterates throug chars and counts instances of \n till end of file
while(!feof(fp))
{
  ch = fgetc(fp);
  if(ch == '\n')
  {
    count_lines++;
  }
}

//create an array of char pointers to store individual line information
//char **ind_lines = (char *)malloc(lines * sizeof(char*))


//capturing the lines 1 by 1
//max 128 chars in a line
char lines[count_lines][128];

int iter=0;

while(iter<count_lines)
{	
	//gets lines
	fgets(lines[iter],128,fp);
	//check for last char
//	if(lines[iter][127]!='\n'
//	//last character
	if(strlen(lines[iter]<128);	//ok, do nothing
	else if(lines[iter][127]!='\n')//whoops
	{
		fprintf(stderr, “Line too long”);
		exit(1);
	}
	iter++;
}

//from copy strtok the word into a the array
//then call qsort(on 2 D array) and return right indices

char *temp;
while(iter<=compare_index)
{

pch=str





