#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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

int main(int argc ,char *argv[])
{	
	char str[80] = "I am  L   GSMabcdefghijklm";
	char *word;
	char str2[128];

	int i;
	for(i=1;i<=6;i++)
	{	
		strcpy(str2,str);
		word=	findWord(str2,i);	printf("Word in main=%s size=%d\n",word,sizeof(word));	
	}

	

	return 0;


}
