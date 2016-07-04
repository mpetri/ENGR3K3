#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	int r;
	int g;
	int b;
} data_t;

typedef struct node node_t;

struct node {
    data_t data;
    node_t *next;
};

typedef struct {
	int num_elements;
	node_t* head;
	node_t* tail;
} list_t;

void
process(data_t d) {
	printf("(%.3d,%.3d,%.3d)\n",d.r,d.g,d.b);
}

void 
process_each(node_t *p) {
    while (p) {
        process(p->data);
        p = p->next;
    }
}

data_t pop_front(list_t* list) {
	node_t* old;
	assert(list->head);
	old = list->head;
	list->head = list->head->next;
	data_t d = old->data;
	free(old);
	list->num_elements--;
	if(list->num_elements == 0) {
		list->head = NULL;
		list->tail = NULL;
	}
	return d;
}


void push_front(list_t* list,data_t d) {
    node_t* new = (node_t*) malloc(sizeof(node_t));
    assert(new);
    new->data = d;
    new->next = list->head;
    list->head = new;
    if(list->tail == NULL) list->tail = new;
	list->num_elements++;
}

void push_back(list_t* list,data_t d) {
    node_t* new = (node_t*) malloc(sizeof(node_t));
    assert(new);
    new->data = d;
    new->next = NULL;
    if(list->tail)
    	list->tail->next = new;
    list->tail = new;
    if(list->head == NULL) list->head = new;
	list->num_elements++;
}

void insert_after(list_t* list,node_t* node,data_t d) {
    /* TODO */
}

list_t* new_list() {
	list_t* lst;
	lst = (list_t*) malloc(sizeof(list_t));
	assert(lst);
	lst->head = NULL;
	lst->tail = NULL;
	lst->num_elements = 0;
	return lst;
}

void free_list(list_t* lst) {
	while(lst->num_elements) {
		pop_front(lst);
	}
	free(lst);
}

int main(int argc, char const *argv[])
{
	list_t* list = new_list();

	for(int i=0;i<5;i++) {
		data_t new_dat;
		new_dat.r = rand() % 255;
		new_dat.g = rand() % 255;
		new_dat.b = rand() % 255;
		push_back(list,new_dat);
	}

	for(int i=0;i<5;i++) {
		data_t new_dat;
		new_dat.r = rand() % 255;
		new_dat.g = rand() % 255;
		new_dat.b = rand() % 255;
		push_front(list,new_dat);
	}

	/* process all elements in list */
	process_each(list->head);

	/* remove everything */
	free_list(list);

	return 0;
}