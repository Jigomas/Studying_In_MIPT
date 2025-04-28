#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void swap(char** a, char** b)
{
	char* x = *a;
	*a = *b;
	*b = x;
}


int myqsort(char** array, int left, int right)
{	
	if (right <= left) return 0;
	
	int pivot = left + rand()%(right-left+1);
	
	 
	
	int r = right;
	swap(&array[pivot], &array[left]);
	pivot = left;
	int l = pivot+1;
	while (l <= r) 
	{	
		if (strcmp(array[l], array[pivot]) < 0)	swap(&array[l++], &array[pivot++]);
		else swap(&array[l], &array[r--]);
	}
	myqsort(array, left, pivot-1);
	myqsort(array, pivot+1, right);
}


int main() 
{
	char buf[4096];
	size_t cap = 4096;
	size_t sz = 0;
	char** array = malloc(cap * sizeof(char*));
	while (fgets(buf, sizeof(buf), stdin)) 
	{
		if (sz >= cap) 
		{
			array = realloc(array, sizeof(char*)*(cap*=2));
			if (!array) return -fprintf(stderr, "Cannot reallocate memory of %lu\n", cap);
		}
		array[sz++] = strdup(buf);
	}
	
	double time_st = clock();
	myqsort(array, 0, sz-1);
	double time_e = clock();
	
	printf("%lf\n", time_e - time_st);
	
	for(int i = 0; i < sz; i++) 
	{
		//fputs(array[i], stdout);
		free(array[i]);
	}
	free(array);
}
