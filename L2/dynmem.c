/*

    Toy program explaining different dynamic memory management functions.
    Use valgrind to check for memory leaks:

        gcc -g -Wall -std=c99 -o dynmem dynmem.c 
        valgrind --leak-check=full ./dynmem

    try commenting out one of the free() calls to check if valgrind finds
    the memory leak.
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

void* safe_malloc(size_t num_bytes)
{
    void* ptr = malloc(num_bytes);
    if (ptr == NULL) {
        printf("ERROR: malloc(%lu)\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* safe_realloc(void* ptr, size_t num_bytes)
{
    void* new_ptr = realloc(ptr, num_bytes);
    if (new_ptr == NULL) {
        printf("ERROR: realloc(%lu)\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

typedef struct {
    int id;
    char* first_name;
    char* last_name;
} record_t;

int main(int argc, char const* argv[])
{
    //// (1) simple one dimensional array
    /* allocate array of 1000 ints */
    size_t num_items = 1000;
    int* array = (int*)safe_malloc(num_items * sizeof(int));

    /* make array bigger */
    num_items = 2000;
    array = (int*)safe_realloc(array, num_items * sizeof(int));

    /* cleanup */
    free(array);

    //// (2) two dimensional array of strings
    size_t num_strings = 1000;
    char** strings = (char**)safe_malloc(num_strings * sizeof(char*));
    for (size_t i = 0; i < num_strings; i++) {
        // dummy string size in [1,512]
        size_t string_size = 1 + rand() % 512;
        strings[i] = (char*)safe_malloc(string_size + 1); // +1 for terminating 0 byte
    }

    /* cleanup */
    // first delete individual string memory
    for (size_t i = 0; i < num_strings; i++) {
        free(strings[i]);
    }
    // second delete the array of string pointers
    free(strings);

    //// (3) complex structures
    size_t num_records = 1000;
    record_t* records = (record_t*)safe_malloc(num_records * sizeof(record_t));
    // for each record we have to manually allocate memory for first and last name
    for (size_t i = 0; i < num_records; i++) {
        // dummy first name len in [1,32]
        size_t first_name_len = 1 + rand() % 32;
        // dummy kast name len in [1,64]
        size_t last_name_len = 1 + rand() % 64;
        records[i].first_name = (char*)safe_malloc(first_name_len + 1); // +1 for terminating 0 byte
        records[i].last_name = (char*)safe_malloc(last_name_len + 1); // +1 for terminating 0 byte
    }

    /* cleanup */
    for (size_t i = 0; i < num_records; i++) {
        free(records[i].first_name);
        free(records[i].last_name);
    }
    free(records);

    return EXIT_SUCCESS;
}