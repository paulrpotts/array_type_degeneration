#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
    This is demonstration code that goes along with my article "On Degeneration of Array Types in C"
    https://thepottshouse.org/paul/portfolio/On_Degeneration_of_Array_Types_in_C.html

    I have provided it solely so that readers can follow along as they read the original article, step
    through it, and experiment with it.

    Steps for building and running this under your computer's debugger will vary, depending on your
    operating system and toolchain.
*/

typedef enum {
    Red, Orange, Yellow, Green, Blue, Indigo, Violet
} color_t;

#define DIM_X 5
#define DIM_Y 7

typedef color_t colors_one_d_a_t[DIM_X];

typedef color_t colors_two_d_a_t[DIM_Y][DIM_X];

colors_two_d_a_t colors_two_d_a =
{
    { Violet, Violet, Violet, Violet, Violet },
    { Indigo, Indigo, Indigo, Indigo, Indigo },
    { Blue, Blue, Blue, Blue, Blue },
    { Green, Green, Green, Green, Green },
    { Yellow, Yellow, Yellow, Yellow, Yellow },
    { Orange, Orange, Orange, Orange, Orange },
    { Red, Red, Red, Red, Red }
};

int main(int argc, const char * argv[])
{
    // Allocate space for a second array
    void * raw_mem_p = malloc( sizeof(colors_two_d_a_t) );

    // Here is a demonstration of how to use a separately-allocated allocated array
    // of pointers to point to allocated blocks of elements, which will
    // allow us to use 2-dimensional array notation.

    color_t ** colors_two_d_pp =
        (color_t **) malloc( DIM_Y * sizeof( color_t * ) );
    for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    {
        colors_two_d_pp[y_idx] = 
            (color_t *)malloc( sizeof( colors_one_d_a_t ) );
        for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
        {
            colors_two_d_pp[y_idx][x_idx] = 
                colors_two_d_a[y_idx][x_idx];
        }
    }

    printf( "elements: %d, %d, %d, %d\n",
        colors_two_d_pp[0][0],
        colors_two_d_pp[1][1],
        colors_two_d_pp[5][3],
        colors_two_d_pp[6][4]);

    // Here is the above code instrumented to demonstrate that the
    // stride lengths are applied as expected when using square
    // brackets.

    printf("Allocated block to hold pointers at %p with size %zu\n",
        colors_two_d_pp, DIM_Y * sizeof( color_t * ));
    for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    {
        colors_two_d_pp[y_idx] = 
            (color_t *)malloc( sizeof( colors_one_d_a_t ) );
        printf("Allocated block to hold elements at %p with size %zu\n",
            colors_two_d_pp[y_idx], sizeof( colors_one_d_a_t ));
        printf("Diff from start of pointers to current pointer: %td\n",
            ( (void *)&colors_two_d_pp[y_idx] - (void *)colors_two_d_pp ) );
        for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
        {
            printf("Diff from start of elements to current element: %td\n",
                (void *)( &colors_two_d_pp[y_idx][x_idx] )
                    - (void *)( &colors_two_d_pp[y_idx][0] ) );
            printf("Address of element, array math  : %p\n",
                &colors_two_d_pp[y_idx][x_idx] );
            printf("Address of element, pointer math: %p\n",
                *( colors_two_d_pp + y_idx ) + x_idx );
        }
    }

    // And here is another way to do the above, using one single
    // allocated block of memory.

    void * colors_alloc_p =
        malloc( DIM_Y * sizeof ( color_t * ) + 
                DIM_Y * DIM_X * sizeof( color_t ) );

    color_t ** row_ptr_a_p = (color_t **)colors_alloc_p;
    color_t * colors_a_p = (color_t *)( colors_alloc_p +
        DIM_Y * sizeof ( color_t * ) );

    for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    {
        row_ptr_a_p[y_idx] = colors_a_p + y_idx * DIM_X;
        for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
        {
            row_ptr_a_p[y_idx][x_idx] = 
                colors_two_d_a[y_idx][x_idx];
        }
    }

    printf( "elements: %d, %d, %d, %d\n",
        row_ptr_a_p[0][0],
        row_ptr_a_p[1][1],
        row_ptr_a_p[5][3],
        row_ptr_a_p[6][4]);

    return 0;
}
