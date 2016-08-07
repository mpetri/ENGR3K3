/*

    Toy program explaining showing different methods to track memory usage.

*/
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int binary_search_recursive(int* A, int lo, int hi, int x)
{
    if (lo >= hi) {
        return -1;
    }
    int mid = (lo + hi) / 2;
    int M = A[mid];
    if (x < M) {
        return binary_search_recursive(A, lo, mid, x);
    } else if (x > M) {
        return binary_search_recursive(A, mid + 1, hi, x);
    } else {
        return mid;
    }
}

int binary_search(int* A, int lo, int hi, int x)
{
    while (lo < hi) {
        int m = (lo + hi) / 2;
        if (x < A[m]) {
            hi = m;
        } else if (x > A[m]) {
            lo = m + 1;
        } else {
            return m;
        }
    }
    return -1;
}

void* safe_malloc(size_t num_bytes)
{
    void* ptr = malloc(num_bytes);
    if (ptr == NULL) {
        printf("ERROR: malloc(%lu)\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int intcmp(const void* a, const void* b)
{
    int ia = *(int*)a;
    int ib = *(int*)b;
    return ia - ib;
}

int main(int argc, char const* argv[])
{
    /* create some input */
    size_t num_items = 10000000;
    int* array = (int*)safe_malloc(num_items * sizeof(int));
    for (size_t i = 0; i < num_items; i++) {
        array[i] = rand();
    }

    /* sort it */
    qsort(array, num_items, sizeof(int), intcmp);

    printf("A[] = ");
    for (size_t i = 0; i < 20; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    /* search for some stuff */
    size_t num_searches = 100000;
    for (size_t i = 0; i < num_searches; i++) {
        int key = rand();
        int ret_recursive = binary_search_recursive(array, 0, num_items, key);
        int ret_iterative = binary_search(array, 0, num_items, key);

        if (ret_iterative != ret_recursive) {
            printf("error searching for %d\n", key);
        }
    }

    /* search for stuff in the array */
    num_searches = 100000;
    for (size_t i = 0; i < num_searches; i++) {
        int key = array[rand() % num_items];
        int ret_recursive = binary_search_recursive(array, 0, num_items, key);
        int ret_iterative = binary_search(array, 0, num_items, key);

        if (ret_iterative != ret_recursive || ret_recursive == -1) {
            printf("error searching for %d. %d %d\n", key, ret_recursive, ret_iterative);
        }
    }

    /* cleanup */
    free(array);

    return EXIT_SUCCESS;
}
