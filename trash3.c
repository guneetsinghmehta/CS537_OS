#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
int cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
	/* strcmp functions works exactly as expected from
	comparison function */ 
}

void print_cstring_array(char **array, size_t len) 
{ 
    size_t i;
 
    for(i=0; i<len; i++) 
        printf("%s | ", array[i]);
 
    putchar('\n');
}

 void sort_cstrings_example() 
{ 
    char *strings[] = { "Zorro", "Alex", "Celine", "Bill", "Forest", "Dexter" };
    size_t strings_len = sizeof(strings) / sizeof(char *);
 	
 	
 	
    /** STRING */ 
    puts("*** String sorting...");
 
    /* print original string array */ 
    print_cstring_array(strings, strings_len);
 
    /* sort array using qsort functions */ 
    qsort(strings, strings_len, sizeof(char *), cstring_cmp);
 
    /* print sorted string array */ 
    print_cstring_array(strings, strings_len);
} 

void sortG(char **strings,int *order,int number)
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
	printf("printing sorted strings\n");
	for(i=0;i<number;i++)
	{
		printf("%s\n",strings[i]);
	}
}

int main()
{
sort_cstrings_example();
   char *strings[] = { "Zorro", "Alex", "Celine", "Bill", "Forest", "Dexter" };
    size_t strings_len = sizeof(strings) / sizeof(char *);
    int i;
    int indices[strings_len];
    for(i=0;i<strings_len;i++){indices[i]=i+1;}
 sortG(strings,indices,strings_len);
 	for(i=0;i<strings_len;i++){printf("%d\n",indices[i]);}
}