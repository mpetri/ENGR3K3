/*

    Toy program explaining showing different methods to track memory usage.

*/
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static uint64_t cur_mem_bytes = 0;
static uint64_t peak_mem_bytes = 0;

void* mymalloc(size_t size)
{
    size_t new_size = size + sizeof(uint64_t);
    void* mem = malloc(new_size);
    assert(mem != NULL);
    uint64_t* mem64 = (uint64_t*)mem;
    *mem64 = size;
    mem64++;
    cur_mem_bytes += size;
    if (cur_mem_bytes > peak_mem_bytes)
        peak_mem_bytes = cur_mem_bytes;
    return (void*)mem64;
}

void* myrealloc(void* ptr, size_t size)
{
    size_t new_size = size + sizeof(uint64_t);
    uint64_t* mem64 = (uint64_t*)ptr;
    mem64--;
    int64_t prev_size = (int64_t)*mem64;
    int64_t diff = (int64_t)size - prev_size;
    void* real_ptr = (void*)mem64;
    void* mem = realloc(real_ptr, new_size);
    assert(mem != NULL);
    mem64 = (uint64_t*)mem;
    *mem64 = size;
    mem64++;
    cur_mem_bytes += diff;
    if (cur_mem_bytes > peak_mem_bytes)
        peak_mem_bytes = cur_mem_bytes;
    return (void*)mem64;
}

void myfree(void* ptr)
{
    uint64_t* mem64 = (uint64_t*)ptr;
    mem64--;
    size_t bytes = *mem64;
    cur_mem_bytes -= bytes;
    void* real_ptr = (void*)mem64;
    free(real_ptr);
}

void* safe_malloc(size_t num_bytes)
{
    void* ptr = mymalloc(num_bytes);
    if (ptr == NULL) {
        printf("ERROR: malloc(%lu)\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* safe_realloc(void* ptr, size_t num_bytes)
{
    void* new_ptr = myrealloc(ptr, num_bytes);
    if (new_ptr == NULL) {
        printf("ERROR: realloc(%lu)\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

void print_mem_usage()
{
    printf("MEMUSAGE: current = %lu peak = %lu\n", cur_mem_bytes, peak_mem_bytes);
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
    myfree(array);

    //// (2) two dimensional array of strings
    size_t num_strings = 1000;
    char** strings = (char**)safe_malloc(num_strings * sizeof(char*));
    for (size_t i = 0; i < num_strings; i++) {
        // dummy string size in [1,512]
        size_t string_size = 1 + rand() % 512;
        strings[i] = (char*)safe_malloc(string_size + 1); // +1 for terminating 0 byte
        print_mem_usage();
    }

    /* cleanup */
    // first delete individual string memory
    for (size_t i = 0; i < num_strings; i++) {
        myfree(strings[i]);
        print_mem_usage();
    }
    // second delete the array of string pointers
    myfree(strings);
    print_mem_usage();

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
        print_mem_usage();
    }

    /* cleanup */
    for (size_t i = 0; i < num_records; i++) {
        myfree(records[i].first_name);
        myfree(records[i].last_name);
        print_mem_usage();
    }
    myfree(records);
    print_mem_usage();

    return EXIT_SUCCESS;
}