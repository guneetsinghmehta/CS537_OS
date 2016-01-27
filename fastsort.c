#include<stdio.h>
#include<string.h>
int main(int argc ,char *argv[])
{
	//Argument Check -starts
	if(argv[1][0]=='-'&&argc==3){}	
	else{fprintf(stderr,"Error: Bad command line parameters");exit(1);}
	//Argument Check -ends
	
	char filename[300];	strcpy(filename,argv[2]);
	FILE *f1;	f1=fopen(filename,"r");
	if(f1<=0){fprintf("Error: Cannot open file %s",filename);exit(1);}
	printf("%d",f1); 										
	char temp[128];
	while(fgets(temp,128,f1)!=NULL)
	{
		printf("%s\n",temp);
	}
	
	return 0;
}
