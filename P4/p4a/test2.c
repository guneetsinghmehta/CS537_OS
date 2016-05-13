#include <pthread.h>
#include <stdio.h>
#include<malloc.h>
#include<string.h>
// implement a mutex
//make 2 consumer threads - takes the value from 
//make 1 producer thread - producer just puts values from 1 to 1000
struct linkJob{
	struct linkJob* next;
	int intData;
	char stringData[];
};

struct linkJob* linkQueque;//always add and remove to the front of the queque. Do not qorry about starvation for now
int linkQuequeLength=0;

void* producerFn(void* arg)
{
	int i;
	struct linkJob *newLinkJob,*temp;
	for(i=0;i<1000;i++)
	{
	
		//printf("%d\n",i);
		newLinkJob=malloc(sizeof(struct linkJob));
		newLinkJob->intData=i;
		//addition to the head
		newLinkJob->next=linkQueque;
		linkQueQue=newLinkJob;
		
		linkQuequeLength++;
		
	}
	return NULL;
}

void* consumerFn(void *arg)
{
	struct linkJob *processLinkJob;// Job to be performed by the consumer
	if(linkQuequeLength!=0)
	{
		processLinkJob=linkQueue;
		linkQueQue=linkQueQue->next;
		printf("process data=%d",processLinkJob->intData);
		linkQueueLength--;
	}
	return NULL;
}

int main(void)
{
	linkQueque=NULL;
	linkQueque=malloc(sizeof(struct linkJob));
	linkQueque->next=NULL;
  	char testData[]="start";
  	strcpy(linkQueque->stringData,testData);
  	linkQueque->intData=1;
	
	//Making producer thread
	pthread_t producer;
	pthread_create(&producer,NULL,producerFn,NULL);
	pthread_join(producer,NULL);
	
	pthread_t consumer;
	pthread_create(&consumer,NULL,consumerFn,NULL);
	pthread_join(consumer,NULL);
}
