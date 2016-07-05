#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "bst.h"

typedef struct {
	char* name;
	char* country;
	int population;
} city_t;

int city_cmp_pol(void* a,void* b) {
	city_t* city_a = (city_t*) a;
	city_t* city_b = (city_t*) b;
	int cmp = city_a->population - city_b->population;
	if(cmp == 0) {
		return strcmp(city_a->name,city_b->name);
	}
	return cmp;
}

void city_free(void* c) {
	city_t* city = (city_t*) c;
	free(city->name);
	free(city->country);
	free(city);
}

city_t* city_new(char* name,char* country,int pop) {
	city_t* c = (city_t*) malloc(sizeof(city_t));
	assert(c != NULL);
	c->name = strdup(name);
	c->country = strdup(country);
	assert(c->name != NULL);
	assert(c->country != NULL);
	c->population = pop;
	return c;
}

void city_print(void* c) {
	city_t* city = (city_t*) c;
	printf("(%45s,%35s,%9d)\n",city->name,city->country,city->population);
}

int main(int argc, char const *argv[])
{
	/* create a new city bst */
	bst_t* bst = bst_new(city_free,city_cmp);

	/* read data and insert into bst */
	int read = 0;
	char name_buf[256];
	char country_buf[256];
	int population;
	while( scanf("%[^,],%[^,],%d\n",name_buf,country_buf,&population) == 3 ) {
		city_t* new_city = city_new(name_buf,country_buf,population);
		int ret = bst_insert(bst,new_city);
		if(ret != BST_SUCCESS) {
			printf("error inserting city. duplicate?");
			city_free(new_city);
		}
		read++;
	}
	printf("parsed %d cities\n",bst->num_elements);
	assert(bst->num_elements == read);

	/* print all cities */
	bst_traverse(bst,BST_INORDER,city_print);

	/* find the largest */
	node_t* largest_city_node = bst_max(bst);
	if(largest_city_node) {
		printf("largest city is:\n");
		city_print(largest_city_node->data);
	}


	node_t* melb_city_node = bst_find()

	return 0;
}