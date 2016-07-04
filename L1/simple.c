#include <stdio.h>

/* define a constant */
#define MAX_POINTS 50000

/* define custom type */
typedef struct {
    float x;
    float y;
} point_t;


int main() {
    /* declare variables */
    point_t A[MAX_POINTS];
    point_t* a_ptr = &A[0];
    int points_read = 0;
    /* read floats from stdin */
    while( points_read < MAX_POINTS && 
        scanf("%f %f\n",
            &(a_ptr->x),&(a_ptr->y)) == 2 ) 
    {
        printf("read (%f,%f)\n",
            a_ptr->x,a_ptr->y);
        points_read++; a_ptr++;
    }
    if(points_read == 0) {
        printf("no points read\n");
        return 0;
    }

    /* find */
    float max_x = A[0].x; 
    float min_y = A[0].y;
    float sum = 0.0f;
    for(int i=1;i<points_read;i++) {
        if(max_x < A[i].x) max_x = A[i].x;
        if(min_y > A[i].y) min_y = A[i].y;
        sum += A[i].x + A[i].y;
    }
    /* print stats */
    printf("%.3f %.3f %.3f\n",
        min_y,max_x,sum);

    return 0;
}