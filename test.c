#include <pthread.h>
#include <stdio.h>
/* This is our thread function.  It is like main(), but for a thread */

struct thread_structure
{
	char c;
	int count;
};

void* thread_function(void* arg)
{
	struct thread_structure* p=(struct thread_structure*) arg;
	printf("character =%c and count=%d\n",p->c,p->count);
	int i;
	for(i=0;i<p->count;i++)
	{
		//printf("%c\n",p->c);
		usleep(1);
	}
	int candidate =2;
	return (void*) candidate;
}

int main(void)
{
	pthread_t thread1,thread2;
	struct thread_structure p,p2;
	p.c='a';	p.count=1000;
	p2.c='b';	p2.count=1000;
	
	pthread_create(&thread1,NULL,thread_function,&p);
	pthread_create(&thread2,NULL,thread_function,&p2);
	usleep(1000);
	int ans1,ans2;
	pthread_join(thread1,(void*)&ans1);//adds the thread to the queque of structures
		printf("ans1= %d\n",ans1);
	pthread_join(thread2,(void*)&ans2);
		printf("ans2= %d\n",ans2);
	printf("ans1= %d\n",ans1);
	usleep(1000);

}
