#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int r;
    int g;
    int b;
} rgb_t;

typedef struct node node_t;

struct node {
    void* data;
    node_t* next;
};

typedef struct {
    int num_elements;
    node_t* head;
    node_t* tail;
    void (*del)(void*);
} list_t;

void rgb_print(void* d)
{
    assert(d != NULL);
    rgb_t* rd = (rgb_t*)d;
    printf("(%.3d,%.3d,%.3d)\n", rd->r, rd->g, rd->b);
}

void list_process_all(node_t* p, void (*process)(void*))
{
    while (p) {
        process(p->data);
        p = p->next;
    }
}

void* list_pop_front(list_t* list)
{
    assert(list != NULL);
    assert(list->num_elements > 0);
    node_t* old;
    assert(list->head != NULL);
    old = list->head;
    list->head = list->head->next;
    void* d = old->data;
    free(old);
    list->num_elements--;
    if (list->num_elements == 0) {
        list->head = NULL;
        list->tail = NULL;
    }
    return d;
}

void list_push_front(list_t* list, void* d)
{
    assert(list != NULL);
    node_t* new = (node_t*)malloc(sizeof(node_t));
    assert(new);
    new->data = d;
    new->next = list->head;
    list->head = new;
    if (list->tail == NULL)
        list->tail = new;
    list->num_elements++;
}

void list_push_back(list_t* list, void* d)
{
    assert(list != NULL);
    node_t* new = (node_t*)malloc(sizeof(node_t));
    assert(new != NULL);
    new->data = d;
    new->next = NULL;
    if (list->tail)
        list->tail->next = new;
    list->tail = new;
    if (list->head == NULL)
        list->head = new;
    list->num_elements++;
}

/* TODO: implement a function inserts a new node (for item d)
		 after node n
*/
void list_insert_after(list_t* list, node_t* n, void* d)
{
    assert(list != NULL);
    assert(n != NULL);
}

/* TODO: implement a function which removes the
		 last element in the list 
*/
void* list_pop_back(list_t* list)
{
    assert(list != NULL);
    assert(list->num_elements > 0);
    return NULL;
}

list_t* list_new(void (*delfunc)(void*))
{
    list_t* list;
    list = (list_t*)malloc(sizeof(list_t));
    assert(list != NULL);
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;
    list->del = delfunc;
    return list;
}

void list_free(list_t* list)
{
    while (list->num_elements) {
        void* elem = list_pop_front(list);
        list->del(elem);
    }
    free(list);
}

int main(int argc, char const* argv[])
{
    /* create new list object with regular 'free' as the del function */
    list_t* list = list_new(free);

    /* insert some elements at the front */
    assert(list->num_elements == 0);
    for (int i = 0; i < 5; i++) {
        rgb_t* new_dat = (rgb_t*)malloc(sizeof(rgb_t));
        new_dat->r = rand() % 255;
        new_dat->g = rand() % 255;
        new_dat->b = rand() % 255;
        list_push_back(list, new_dat);
    }
    assert(list->num_elements == 5);

    /* insert some elements at the back */
    for (int i = 0; i < 5; i++) {
        rgb_t* new_dat = (rgb_t*)malloc(sizeof(rgb_t));
        new_dat->r = rand() % 255;
        new_dat->g = rand() % 255;
        new_dat->b = rand() % 255;
        list_push_front(list, new_dat);
    }
    assert(list->num_elements == 10);

    /* process all elements in list */
    list_process_all(list->head, rgb_print);

    /* remove everything */
    list_free(list);

    return 0;
}