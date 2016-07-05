#include <stdio.h>
#include <stdint.h>

int main(int argc, char const* argv[])
{
    double A[10];
    const char* str = "mississippi";
    printf("sizeof(char)      = %2lu\n", sizeof(char));
    printf("sizeof(int)       = %2lu\n", sizeof(int));
    printf("sizeof(float)     = %2lu\n", sizeof(float));
    printf("sizeof(double)    = %2lu\n", sizeof(double));
    printf("sizeof(A)         = %2lu\n", sizeof(A));
    printf("sizeof(*A)        = %2lu\n", sizeof(*A));
    printf("sizeof(p)         = %2lu\n", sizeof(str));
    printf("sizeof(*p)        = %2lu\n", sizeof(*str));
    printf("sizeof(uint8_t)   = %2lu\n", sizeof(uint8_t));
    printf("sizeof(uint16_t)  = %2lu\n", sizeof(uint16_t));
    printf("sizeof(uint32_t)  = %2lu\n", sizeof(uint32_t));
    printf("sizeof(uint64_t)  = %2lu\n", sizeof(uint64_t));
    printf("sizeof(int64_t)   = %2lu\n", sizeof(int64_t));
    return 0;
}