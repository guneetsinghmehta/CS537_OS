#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void findWord(char *line,char *token,int wordIndex)
{
	//puts wordIndex th number in word from the line 
	int i,j;
	const char s[2] = " ";
//	char *token;
	char *lastToken;
	token=strtok(line,s);i=1;

	while(i<=wordIndex&&token!=NULL)
	{
		lastToken=token;
		if(i==wordIndex)
			printf("%s %d \n",token,i);
		token=strtok(NULL,s);
		i++;	
	}
	//word=lastToken;
}

int main(int argc ,char *argv[])
{	
	char str2[80] = "I am  L   GSM";
	char *str;
   const char s[2] = " ";
   /*
   char *token;
   printf("%d",sizeof(str));

   token = strtok(str, s);
   while( token != NULL ) 
   {
      printf( " %s%d\n", token,strlen(token) );
    
      token = strtok(NULL, s);
   }
   */
   char *word;
   int i;
   for(i=1;i<=4;i++)
   {
   	strcpy(str,str2);
   	findWord(str,word,i);
   	//printf("%s %d",word,i);	
   }
   
   
   
   return(0);

}
