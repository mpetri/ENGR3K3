#include "bst.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* create a new empty bst structure */
bst_t* bst_new(void (*delfunc)(void*),int (*cmpfunc)(void*,void*))
{
    bst_t* bst;
    bst = (bst_t*) malloc(sizeof(bst_t));
    assert(bst != NULL);
    bst->root = NULL;
    bst->num_elements = 0;
    bst->del = delfunc;
    bst->cmp = cmpfunc;
    return bst;
}

/* free all memory assocated with a subtree */
void bst_free_subtree(bst_t* bst,node_t* n) {
	assert(bst != NULL);
	if(n) {
		bst_free_subtree(bst,n->left);
		bst_free_subtree(bst,n->right);
		bst->del(n->data);
		free(n);
		bst->num_elements--;
	}
}

/* free all memory associated with a bst */
void bst_free(bst_t* bst) {
	assert(bst != NULL);
	bst_free_subtree(bst,bst->root);
	free(bst);
}

/* insert a new element into the bst */
int bst_insert(bst_t* bst,void* d) {
	assert(bst != NULL);
	assert(d != NULL);
	node_t* parent = NULL;
	node_t* tmp = bst->root;
	while(tmp) {
		parent = tmp;
		if(bst->cmp(tmp->data,d) > 0) { // element is smaller
			tmp = tmp->left;
		} else if(bst->cmp(tmp->data,d) < 0) { // element is bigger
			tmp = tmp->right;
		} else {
			/* ALREADY EXISTS! -> do nothing and return ERROR */
			return BST_FAILURE;
		}
	}

	/* insert as child of parent */
	node_t* new_node = (node_t*) malloc(sizeof(node_t));
	assert(new_node != NULL);
	new_node->data = d;
	new_node->left = NULL;
	new_node->right = NULL;
	if(parent) {
		if(bst->cmp(parent->data,d) > 0) { // element is smaller
			parent->left = new_node;
		} else {
			parent->right = new_node;
		}
	} else { 
		bst->root = new_node; 
	}
	bst->num_elements++;

	return BST_SUCCESS;
}

int bst_remove(bst_t* bst,void* d) {
	assert(bst != NULL);
	assert(d != NULL);
	node_t* parent = NULL;
	node_t* tmp = bst->root;
	while(tmp) {
		parent = tmp;
		if(bst->cmp(tmp->data,d) > 0) { // element is smaller
			tmp = tmp->left;
		} else if(bst->cmp(tmp->data,d) < 0) { // element is bigger
			tmp = tmp->right;
		} else {
			/* FOUND! */
			break;
		}
	}

	if(tmp) {
		/* leaf? */
		if(tmp->left == NULL && tmp->right == NULL) {
			if(parent->left == tmp)
				parent->left = NULL;
			if(parent->right == tmp)
				parent->right = NULL;
		} else {
			/* one of the subtrees empty? */
			if(tmp->left == NULL) {
				if(parent->left == tmp)
					parent->left = tmp->right;
				if(parent->right == tmp)
					parent->right = tmp->right;
			}
			else if(tmp->right == NULL) {
				if(parent->left == tmp)
					parent->left = tmp->left;
				if(parent->right == tmp)
					parent->right = tmp->left;
			} else {
				/* both subtrees full? */

				/* (1) find min in right subtree */
				node_t* min_tmp = tmp->right;
				while(min_tmp && min_tmp->left) {
					min_tmp = min_tmp->left;
				}
				/* (2) attach left subtree of node to remove to min of right subtree */
				min_tmp->left = tmp->left;

				/* (3) attach to parent */
				if(parent->right == tmp)
					parent->left = tmp->right;
				if(parent->right == tmp)
					parent->right = tmp->right;
			}
			
		}

		/* free memory */
		bst->del(tmp->data);
		free(tmp);
		return BST_SUCCESS;
	}

	return BST_FAILURE;
}

/* find */
node_t* bst_find(bst_t* bst,void* d) {
	assert(bst != NULL);
	assert(d != NULL);
	node_t* tmp = bst->root;
	while(tmp) {
		if(bst->cmp(tmp->data,d) > 0) { // element is smaller
			tmp = tmp->left;
		} else if(bst->cmp(tmp->data,d) < 0) { // element is bigger
			tmp = tmp->right;
		} else {
			/* FOUND! */
			break;
		}
	}
	return tmp;
}

node_t* bst_recursive_find(bst_t* bst,node_t* n,void* d) {
	assert(bst != NULL);
	assert(d != NULL);
	if(n) {
		if(bst->cmp(n->data,d) > 0) { // element is smaller
			return bst_recursive_find(bst,n->left,d);
		} else if(bst->cmp(n->data,d) < 0) { // element is bigger
			return bst_recursive_find(bst,n->right,d);
		} else {
			return n;
		}
	}
	return NULL;
}

int bst_contains(bst_t* bst,void* d) {
	assert(bst != NULL);
	assert(d != NULL);
	node_t* n = bst_recursive_find(bst,bst->root,d);
	if(n != NULL)
		return BST_SUCCESS;
	return BST_FAILURE;
}

/* traversal */
void bst_traverse(bst_t* bst,int BST_ORDER,void (*process)(void*)) {
	assert(bst != NULL);
	switch(BST_ORDER) {
		case BST_PREORDER:
			bst_traverse_preorder(bst->root,process);
			break;
		case BST_INORDER:
			bst_traverse_inorder(bst->root,process);
			break;
		case BST_POSTORDER:
			bst_traverse_postorder(bst->root,process);
			break;
		default:
			printf("ERROR: unknown bst traversal order.\n");
			exit(EXIT_FAILURE);
	}
}

void bst_traverse_preorder(node_t* n,void (*process)(void*)) {
	if(n) {
		process(n->data);
		bst_traverse_preorder(n->left,process);
		bst_traverse_preorder(n->right,process);
	}
}
void bst_traverse_inorder(node_t* n,void (*process)(void*)) {
	if(n) {
		bst_traverse_preorder(n->left,process);
		process(n->data);
		bst_traverse_preorder(n->right,process);
	}
}
void bst_traverse_postorder(node_t* n,void (*process)(void*)) {
	if(n) {
		bst_traverse_preorder(n->left,process);
		bst_traverse_preorder(n->right,process);
		process(n->data);
	}
}

/* advanced stuff */
node_t* bst_successor(bst_t* bst,void* d) {
	assert(bst != NULL);
	assert(d != NULL);
	return NULL;
}

node_t* bst_min(bst_t* bst) {
	assert(bst != NULL);
	node_t* tmp = bst->root;
	while(tmp && tmp->left) {
		tmp = tmp->left;
	}
	return tmp;
}

node_t* bst_max(bst_t* bst) {
	assert(bst != NULL);
	node_t* tmp = bst->root;
	while(tmp && tmp->right) {
		tmp = tmp->right;
	}
	return tmp;
}
