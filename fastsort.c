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
	if(argv[1][0]=='-'&&argc==3){printf("Correct\n");}	
	else{fprintf(stderr,"Error: Bad command line parameters");exit(1);}
	//Argument Check -ends
	
	//file handling
	char filename[300];	
	strcpy(filename,argv[2]);
	FILE *f1;	
	f1=fopen(filename,"r");
	if(fileno(f1)<=0){fprintf(stderr,"Error: Cannot open file %s",filename);exit(1);}
	printf("%d\n",fileno(f1)); 				
	
	//reading file into lines array and word array						
	char temp[MAX],wordTemp[MAX];
	int numLines;
	numLines=numLinesfn(filename);
	char lines[numLines][MAX];
	char wordss[numLines][MAX];
	int i=0;
	while(fgets(temp,MAX,f1)!=NULL)
	{

		strcpy(&lines[i][0],temp);
		getWord(&line[i][0],wordTemp,wordIndex);
		printf("%s",&lines[i][0]);
		i++;
	}

	printf("num of lines in file =%d word to be used =%d",numLines,abs(atoi(argv[1])));
	return 0;
} 	

void getWord(char *line,char *word,int wordIndex)
{
	int i,j,blanks=0;
	int blankBegin,blackEnd;
	blankBegin=-1;blankEnd=-1;
	for(i=0;i<MAX;i++)
	{
		if(line[i]==' ')
		{
			if(blankBegin==blackEnd){blankBegin=i;blankEnd=i;}
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
