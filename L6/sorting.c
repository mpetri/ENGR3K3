#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <assert.h>

/*
	MERGE SORT IMPLEMENTATION
 */
void merge(void** array, int n, int mid, int cmp(const void*, const void*))
{
    // (0) need extra space for merging
    void** tmp = malloc(n * sizeof(void*));
    void** left = array;
    void** right = array + mid;
    int i = 0;
    int j = 0;
    int left_size = mid;
    int right_size = n - mid;
    // (1) perform the merge
    for (int k = 0; k < n; k++) {
        if (j == right_size)
            tmp[k] = left[i++];
        else if (i == left_size)
            tmp[k] = right[j++];
        else if (cmp(left[i], right[j]) < 1)
            tmp[k] = left[i++];
        else
            tmp[k] = right[j++];
    }
    // (2) copy the merged array
    for (int i = 0; i < n; i++) {
        array[i] = tmp[i];
    }
    // (3) clean up
    free(tmp);
}

void merge_sort(void** array, int n, int cmp(const void*, const void*))
{
    if (n > 1) {
        int mid = n / 2;
        merge_sort(array, mid, cmp);
        merge_sort(array + mid, n - mid, cmp);
        merge(array, n, mid, cmp);
    }
}

/*
	QUICK SORT IMPLEMENTATION 
 */
void swap_ptr(void** array, int i, int j)
{
    void* tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

int partition(void** array, int n, int cmp(const void*, const void*))
{
    // (1) pick a good pivot
    swap_ptr(array, 0, rand() % n);
    void* pivot = array[0];
    // (2) swap things around
    int i = 1;
    int j = n - 1;
    while (i <= j) {
        // move i to the right. make sure we don't go out of bounds!
        while (i < n && cmp(pivot, array[i]) > 0) {
            i = i + 1;
        }
        while (cmp(pivot, array[j]) < 0) { // move j to the left
            j = j - 1;
        }
        if (j < i)
            break;
        swap_ptr(array, i, j);
        i++;
        j--;
    }
    swap_ptr(array, 0, j); // swap pivot in the right position
    return j;
}

/* quicksort function with a standard interface */
void quick_sort(void** array, int n, int cmp(const void*, const void*))
{
    if (n > 1) {
        int pivot = partition(array, n, cmp);
        quick_sort(array, pivot, cmp);
        quick_sort(array + pivot + 1, n - (pivot + 1), cmp);
    }
}

/* some custom struct we are sorting */
typedef struct {
    int age;
    int id;
} student_t;

student_t* create_random_student()
{
    student_t* new_student = (student_t*)malloc(sizeof(student_t));
    assert(new_student != NULL);
    new_student->age = 18 + rand() % 30;
    new_student->id = rand();
    return new_student;
}

int student_id_cmp(const void* a, const void* b)
{
    student_t* sa = (student_t*)a;
    student_t* sb = (student_t*)b;
    // with int data we can just subtract to get the right behaviour
    return sa->id - sb->id;
}

// qsort gets a pointer to the pointer elements
int qsort_student_id_cmp(const void* a, const void* b)
{
    student_t** sa = (student_t**)a;
    student_t** sb = (student_t**)b;
    // with int data we can just subtract to get the right behaviour
    return (*sa)->id - (*sb)->id;
}

void print_time_ms(const char* name, struct timeval start, struct timeval stop)
{
    double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
    elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
    printf("%s elapsed time = %.2f ms\n", name, elapsed_ms);
}

void check_is_sorted(void** array, int n, int cmp(const void*, const void*))
{
    for (int i = 1; i < n; i++) {
        void* prev = array[i - 1];
        void* cur = array[i];
        if (cmp(prev, cur) > 0) {
            printf("SORTING ERROR %d!\n", i);
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char const* argv[])
{
    for (int n = 10; n <= 10000000; n *= 10) {
        printf("creating %d elements\n", n);
        student_t** student_data = (student_t**)malloc(n * sizeof(student_t*));
        assert(student_data != NULL);
        for (int i = 0; i < n; i++) {
            student_data[i] = create_random_student();
        }
        struct timeval start;
        struct timeval stop;
        printf(" sorting %d elements\n", n);

        gettimeofday(&start, NULL);
        quick_sort((void**)student_data, n, student_id_cmp);
        gettimeofday(&stop, NULL);
        check_is_sorted((void**)student_data, n, student_id_cmp);
        print_time_ms("\tquick_sort", start, stop);

        gettimeofday(&start, NULL);
        merge_sort((void**)student_data, n, student_id_cmp);
        gettimeofday(&stop, NULL);
        check_is_sorted((void**)student_data, n, student_id_cmp);
        print_time_ms("\tmerge_sort", start, stop);

        gettimeofday(&start, NULL);
        qsort(student_data, n, sizeof(student_t*), qsort_student_id_cmp);
        gettimeofday(&stop, NULL);
        check_is_sorted((void**)student_data, n, student_id_cmp);
        print_time_ms("\tqsort", start, stop);

        // cleanup
        printf("cleanup!\n");
        for (int i = 0; i < n; i++)
            free(student_data[i]);
        free(student_data);
    }
    return 0;
}