/*
	Sample file which shows file I/O
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define MAX_BUF_LEN 2048

int file_exists(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    }
    fclose(fp);
    return 1;
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

FILE* safe_fopen(const char* path, const char* mode)
{
    FILE* fp = fopen(path, mode);
    if (fp == NULL) {
        perror("file open error.");
        exit(EXIT_FAILURE);
    }
    return fp;
}

size_t file_size_in_bytes(FILE* fp)
{
    assert(fp != NULL);
    long cur_pos = ftell(fp);
    if (cur_pos == -1) {
        perror("ftell error.");
        exit(EXIT_FAILURE);
    }
    /* seek to the end */
    int ret = fseek(fp, 0L, SEEK_END);
    if (ret == -1) {
        perror("file seek error.");
        exit(EXIT_FAILURE);
    }
    long fs = ftell(fp);

    /* seek back to the previous position */
    ret = fseek(fp, cur_pos, SEEK_SET);
    if (ret == -1) {
        perror("file seek error.");
        exit(EXIT_FAILURE);
    }
    return (size_t)fs;
}

int main(int argc, char const* argv[])
{
    if (argc != 3) {
        printf("USAGE: %s <in file name> <out_file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* file_name = argv[1];
    const char* out_file_name = argv[2];
    {
        /* EXAMPLE 1) read line by line */
        FILE* fp = safe_fopen(file_name, "r");
        char buf[MAX_BUF_LEN];
        uint32_t lines_read = 0;
        while (fgets(buf, MAX_BUF_LEN, fp) != NULL) {
            printf("read line %u: %s", ++lines_read, buf);
        }
        printf("read %u lines\n", lines_read);
        fclose(fp);
    }
    {
        /* EXAMPLE 2) assume file contains lines with 2 integers separated by ; (eg. 123123;4394823) */
        FILE* fp = safe_fopen(file_name, "r");
        int first, second;
        uint32_t points_read = 0;
        while (fscanf(fp, "%d;%d\n", &first, &second) == 2) {
            printf("read point <%d,%d>\n", first, second);
            points_read++;
        }
        printf("read %u points\n", points_read);
        fclose(fp);
    }
    {
        /* EXAMPLE 3) read whole file into buffer */
        FILE* fp = safe_fopen(file_name, "r");
        size_t file_size = file_size_in_bytes(fp);
        char* buf = (char*)safe_malloc(file_size * sizeof(char));
        if (fread(buf, sizeof(char), file_size, fp) != file_size) {
            perror("file read error");
            exit(EXIT_FAILURE);
        }
        /* DO STUFF WITH BUF HERE */
        fclose(fp);
        free(buf);
    }
    {
        /* EXAMPLE 4) write to file using printf */
        FILE* fp = safe_fopen(out_file_name, "w");
        size_t write_lines = 5000;
        for (size_t i = 0; i < write_lines; i++) {
            /* generate two random numbers */
            int first = rand();
            int second = rand();
            if (fprintf(fp, "%d;%d\n", first, second) < 0) {
                perror("file write error");
                exit(EXIT_FAILURE);
            }
        }
        printf("wrote %lu lines\n", write_lines);
        fclose(fp);
    }
    {
        /* EXAMPLE 4) write to file using fwrite */
        FILE* fp = safe_fopen(out_file_name, "w"); // this overwrites the previous content as we are not appending
        const char* message = "abcdefg\n";
        size_t len = strlen(message);
        int written = fwrite(message, sizeof(char), len, fp);
        if (written != len) {
            perror("fwrite error");
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    }

    {
        /* EXAMPLE 5) append to file using fwrite */
        FILE* fp = safe_fopen(out_file_name, "a"); // this appends
        const char* message = "abcdefg\n";
        size_t len = strlen(message);
        int written = fwrite(message, sizeof(char), len, fp);
        if (written != len) {
            perror("fwrite error");
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    }

    return EXIT_SUCCESS;
}