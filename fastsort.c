#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int numLines(char*);
int main(int argc ,char *argv[])
{
	//Argument Check -starts
	if(argv[1][0]=='-'&&argc==3){printf("Correct\n");}	
	else{fprintf(stderr,"Error: Bad command line parameters");exit(1);}
	//Argument Check -ends
	
	char filename[300];	strcpy(filename,argv[2]);
	FILE *f1;	f1=fopen(filename,"r");
	if(fileno(f1)<=0){fprintf(stderr,"Error: Cannot open file %s",filename);exit(1);}
	printf("%d\n",fileno(f1)); 										
	char temp[128];
	while(fgets(temp,128,f1)!=NULL)
	{
		printf("%s",temp);
	}
	printf("num of lines in file =%d word to be used =%d",numLines(filename),abs(atoi(argv[1])));
	return 0;
} 	

int numLines(char *filename)
{
	FILE *f1;char c;
	int numOfLines=0;
	f1=fopen(filename,"r");
	do
	{
		c=fgetc(f1);
		if(c=='\n'){numOfLines++;}
	}while(c!=EOF);
	//if(c!='\n'&&numOfLines>0){numOfLines++;}//because last line does not have a new line character . Skip this because every line is separated by a new line
	fclose(f1);
	return numOfLines;
}
