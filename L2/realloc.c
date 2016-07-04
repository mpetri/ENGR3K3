#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 128

int main(int argc, char const *argv[])
{
	int buf_size = INITIAL_SIZE;
	int* data = (int*) malloc(sizeof(int)*buf_size);
	if(data == NULL) {
		printf("error allocating memory!\n");
		exit(EXIT_FAILURE);
	}
	/* read the data */
	int num = 0;
	int read = 0;
	while( scanf("%d",&num) == 1 ) {
		if(read+1 == buf_size) {
			buf_size = buf_size * 2;
			data = realloc(data,sizeof(int)*buf_size);
			if(data == NULL) {
				printf("error allocating memory!\n");
				exit(EXIT_FAILURE);
			}
			printf("realloc to %d successful\n",buf_size);
		}
		data[read++] = num;
	}
	/* shrink to correct size */
	data = realloc(data,read*sizeof(int));
	if(data == NULL) {
		printf("error shrinking memory!\n");
		exit(EXIT_FAILURE);
	} else {
		printf("shrink to %d successful\n",read);
	}

	/* NOW DO STUFF WITH DATA */
	int sum = 0;
	int64_t sum64 = 0;
	for(int i=0;i<read;i++) {
		sum += data[i];
		sum64 += data[i];
	}
	printf("sum = %d\n",sum); // OVERFLOW!
	printf("sum64 = %ld\n",sum64);
 
	/* always free at the end */
	free(data);
	return 0;
}