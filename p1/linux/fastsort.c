/* Anmol Mohanty
 * P1 - Fast sort
 * All rights reserved
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//structure for key value pair
typedef struct Hashmap {
    int key;
    char *value;
} Hashmap;

//function prototype
static int comp_strings(const void *p1, const void *p2);


int main(int argc, char* argv[])
{

//printf("arg counts=%d",argc);
//arguments check logic
//invalid number of arguments //throw error
if(argc<2 || argc >3)
{
//error message tp stderror - mentioned on webpage
fprintf(stderr, "Error: Bad command line parameters\n");
//clean exit
exit(1);
}

else
{
//stores the index of the word that will be used for comparision
int compare_index=0;//the default
if(argc!=1)
//take care of the hyphen
compare_index=abs(atoi(argv[1]))-1;
//printf("compare_index %d",compare_index);

FILE *fp;

//file opens
if(argc==3){
fp = fopen(argv[2],"r");

if(atoi(argv[1])==0){
fprintf(stderr, "Error: Bad command line parameters\n");
exit(1);}
if(fp==NULL)
{
fprintf(stderr, "Error: Cannot open file %s\n",argv[2]);
exit(1);
}

}
else{
fp = fopen(argv[1],"r");
if(fp==NULL)
{
fprintf(stderr, "Error: Cannot open file %s\n",argv[1]);
exit(1);
}
}
//get number of lines in file

int count_lines=0;
//iterates throug chars and counts instances of \n till end of file
int ch=0;
while(!feof(fp))
{
  ch = fgetc(fp);
  if(ch == '\n')
  {
    count_lines++;
  }
}

//rewind file to the beginning
rewind(fp);

//create an array of char pointers to store individual line information
//char **ind_lines = (char *)malloc(lines * sizeof(char*))


//capturing the lines 1 by 1
//max 128 chars in a line
//char *lines[128] = (char *)malloc(count_lines);

//capturing the individual lines
char **lines = (char**) calloc(count_lines, sizeof(char*));
char **lines_cpy = (char**) calloc(count_lines, sizeof(char*)); //copy to tokenize
if(lines==NULL || lines_cpy==NULL)
{
fprintf(stderr,"malloc failed\n");
exit(1);
}

int i=0;
for ( i = 0; i < count_lines; i++ )
{
    lines[i] = (char*) calloc(128, sizeof(char)); //max length = 128
    lines_cpy[i] = (char*) calloc(128, sizeof(char)); //max length = 128
//	printf("allocing memory for line #%d\n",i);
	if(lines[i]==NULL || lines_cpy[i]==NULL)
	{
		fprintf(stderr,"malloc failed\n");
		exit(1);
	}
}

int iter=0;
//BEGIN FGET
while(iter<count_lines)
{	
	//gets lines
	if(fgets(lines[iter],128,fp)==NULL){
	 fprintf(stderr,"fgets failed\n");
	 exit(1);
	}
//	printf("Hello %s\n",lines[iter]);
	//check for last char
	//fgets(lines[iter],128,fp);
	if(strlen(lines[iter])<127);
	// printf("%s\n",lines[iter]);	//ok, do nothing
	else if(lines[iter][127]!='\n')//whoops
	{
		fprintf(stderr, "Line too long\n");
		exit(1);
	}
	iter++;
}

//from copy strtok the word into a the array
//then call qsort(on 2 D array) and return right indices

//copy string to the copy
for(i=0;i<count_lines;i++)
{
strcpy(lines_cpy[i],lines[i]);
}
//printf("compare=%d\n\n",compare_index);
for(i=0;i<count_lines;i++)
{
   /* get the first token, delim is a space */
   char *token = strtok(lines_cpy[i], " ");
   int iter=0;
   char *last=""; //empty string default value, if nostring or space strings
   /* walk through other tokens */
   while( token != NULL && iter <compare_index ) 
   {	
      last=token;
      //printf( " %s\n", token );
      token = strtok(NULL, " ");
	iter++;
   }

if(token==NULL)//last word reached
lines_cpy[i]=last;
else
lines_cpy[i]=token;
}
//puts("");
//puts("TOKENS");
//for(i=0;i<count_lines;i++)
//printf("%s\n",lines_cpy[i]);

Hashmap tosort[count_lines];

for(i=0;i<count_lines;i++)
{
tosort[i].key=i;
tosort[i].value=lines_cpy[i];
}


//begin sorting
//in place sorting
qsort(tosort, count_lines, sizeof(Hashmap),comp_strings);
//puts("sorted tokens");
//for(i=0;i<count_lines;i++)
//ask if this is ok
//printf("%s\n",lines_cpy[(tosort[i].key)]);
//
//printf("OUTPUT\n");
//jettison the output
for(i=0;i<count_lines;i++)
//ask if this is ok
//puts("CDvcd");
//fprintf(stderr,"%s",lines[(tosort[i].key)]);
printf("%s",lines[(tosort[i].key)]);
//puts("dsnv");
//close the file
//for(i=0;i<count_lines;i++){
//free(lines[i]);
//free(lines_cpy[i]);
//}
fclose(fp);
}
exit(0);
}

static int comp_strings(const void *p1, const void *p2)
{
  return (strcmp((*(Hashmap *)p1).value,(*(Hashmap *)p2).value));
}
