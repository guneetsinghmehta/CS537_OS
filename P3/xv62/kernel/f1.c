#include<stdio.h>
#include<f3.h>
void inc(int *a)
{
	*a++;
}

int main()
{
	a=2;
	inc(&a);
}


