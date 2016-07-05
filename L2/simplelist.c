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
    node_t* next;
};

void process(data_t d)
{
    printf("(%.3d,%.3d,%.3d)\n", d.r, d.g, d.b);
}

void process_each(node_t* p)
{
    while (p) {
        process(p->data);
        p = p->next;
    }
}

node_t*
push(data_t stuff, node_t* p)
{
    node_t* new = (node_t*)malloc(sizeof(node_t));
    assert(new);
    new->data = stuff;
    new->next = p;
    return new;
}

node_t*
pop(node_t* p)
{
    node_t* old;
    assert(p);
    old = p;
    p = p->next;
    free(old);
    return p;
}

int main(int argc, char const* argv[])
{
    node_t* head = NULL;

    for (int i = 0; i < 50; i++) {
        data_t new_dat;
        new_dat.r = rand() % 255;
        new_dat.g = rand() % 255;
        new_dat.b = rand() % 255;
        head = push(new_dat, head);
    }

    /* process all elements in list */
    process_each(head);

    /* remove everything */
    while (head) {
        head = pop(head);
    }

    return 0;
}