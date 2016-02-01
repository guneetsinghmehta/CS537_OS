#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
# define MAX 128
int numLinesfn(char*);
char* findWord(char*,int);
void sortWords (char**,int*,int);
int main(int argc ,char *argv[])
{
	//tasks remaining - malloc
	
	//Argument Check -starts
	char filename[300],temp[MAX];
	int wordIndex,numLines;
	int i;

	if(argc==2){strcpy(filename,argv[1]);wordIndex=1;}
	else if(argc==3&&argv[1][0]=='-')
	{
		strcpy(filename,argv[2]);
		wordIndex=abs(atoi(argv[1]));
		//printf("normal 3");
	}

	else 
	{
		fprintf(stderr,"Error: Bad command line parameters\n");exit(1);
	} 
	//printf("wordIndex=%d and filename=%s",wordIndex,filename);
	//Argument Chek -ends
	
	//opening file -starts
	FILE *f1;
	f1=fopen(filename,"r");
	if(fileno(f1)<=0){fprintf(stderr,"Error: Cannot open file %s",filename);exit(1);}
	
	i=0;
	while(fgets(temp,MAX,f1)!=NULL)	
	{	
	//	printf("%s",temp);	
		if(strlen(temp)>=128)
		{
			fprintf(stderr,"Line too long");exit(1);
		}
		i++;
	}
	numLines=i;
	//opening file -ends
	
	//allocating space for lines - starts	
	char **lines;
	char **words;
	lines=malloc(numLines*sizeof(char*));
	if(lines==NULL)
	{fprintf(stderr,"malloc failed");exit(1);}
	words=malloc(numLines*sizeof(char*));
	if(words==NULL)
	{fprintf(stderr,"malloc failed");exit(1);}
	rewind(f1);i=0;

	//printf("\n");
	while(fgets(temp,MAX,f1)!=NULL)
	{
		lines[i]=malloc(sizeof(temp));
		strcpy(lines[i],temp);
		words[i]=malloc(128*sizeof(char));
		strcpy(words[i],findWord(temp,wordIndex));
		*temp=NULL;
		//printf("%s",lines[i]);
		//printf("%s\n",words[i]);
		i++;
	}
	//printf("printing lines\n");
	//for(i=0;i<numLines;i++){printf("%s",lines[i]);}
	
	//printf("\nprinting words\n");
	//for(i=0;i<numLines;i++){printf("%s\n",words[i]);}
	//allocating space for lines - ends
	
	//sorting starts
	int indices[numLines];
	for(i=0;i<numLines;i++){indices[i]=i;}
	
//	for(i=0;i<numLines;i++){printf("%s\n",words[i]);}
	sortWords(words,indices,numLines);
	//sorting ends
	
	//printf("\nfinal results\n");
	for(i=0;i<numLines;i++)
	{
		printf("%s",lines[indices[i]]);
	}
	return 0;
} 

void sortWords(char **strings,int *order,int number)
{
	int i,j,temp2;
	char *temp;
	for(i=0;i<number;i++)
	{
		for(j=i;j<number;j++)
		{
			if(strcmp(strings[i],strings[j])>0)
			{
				temp=strings[i];strings[i]=strings[j];strings[j]=temp;
				temp2=order[i];order[i]=order[j];order[j]=temp2;
			}	
		}
	}
	//printf("printing sorted strings\n");
	for(i=0;i<number;i++)
	{
		//printf("%s\n",strings[i]);
	}
}	

char * findWord(char *str,int index)
{
   	const char s[2] = " ";
   	   char *token;
   	   char *lastToken;
   token = strtok(str, s);
   int i=1;
   while( token != NULL&&i<=index ) 
   {
	if(i==index)
	{
		//printf("ans=%s\n",token);
	}
      //printf( " %s %d\n", token,strlen(token) );
    lastToken=token;
      token = strtok(NULL, s);
      i++;
   }
   return(lastToken);
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

/*
	Routine to check the file size
	int file=0;
        if((file=open(filename,O_RDONLY)) < -1)
            return 1;
	struct stat fileStat;
	if(fstat(file,&fileStat) < 0)    
		return 1;
	printf("\nfile size=%d ",fileStat.st_size);
	
*/
