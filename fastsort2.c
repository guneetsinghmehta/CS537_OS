#include<stdio.h>
#include<string.h>
#include<stdlib.h>
# define MAX 128
int numLinesfn(char*);
void getWord(char* ,char*,int);
int main(int argc ,char *argv[])
{
	//tasks remaining - malloc
	
	//Argument Check -starts
	char filename[300], *c;
	int wordIndex;
	int i,j,k,temp;

	if(argc==2){strcpy(filename,argv[1]);wordIndex=1;}
	else if(argc==3&&argv[1][0]=='-'&&strtol(&argv[1][1],c,0)>0)
	{
		strcpy(filename,argv[2]);
		wordIndex=strtol(&argv[1][1],c,0);
		printf("normal 3");
	}
	else if(argc==3&&argv[1][0]=='-'&&strtol(&argv[1][1],c,0)==0)
	{
		strcpy(filename,argv[2]);wordIndex=strtol(&argv[1][1],c,0);
		wordIndex=1;
		printf("abnormal 3");
	}
	else 
	{
		fprintf(stderr,"Error: Bad command line parameters");exit(1);
	} 
	printf("wordIndex=%d and filename=%s",wordIndex,filename);
	FILE *f1;
	
	return 0;
} 	

void getWord(char *line,char *word,int wordIndex)
{
	int i,j,blanks=0;
	int blankBegin,blankEnd;
	blankBegin=-1;blankEnd=-1;
	for(i=0;i<MAX;i++)
	{
		if(line[i]==' ')
		{
			if(blankBegin==blankEnd){blankBegin=i;blankEnd=i;}
			else{blankEnd++;}
		}
		else
		{blankBegin=-1;blankEnd=-1;}
	}
	
}

int numLinesfn(char *filename)
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
