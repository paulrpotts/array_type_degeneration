#include <stdio.h>

/*
    This is demonstration code that goes along with my article "On Degeneration of Array Types in C"
    https://thepottshouse.org/paul/portfolio/On_Degeneration_of_Array_Types_in_C.html
    
    I have provided it solely so that readers can follow along as they read the original article, step
    through it, and experiment with it.

    Steps for building and running this under your computer's debugger will vary, depending on your
    operating system and toolchain.
*/

/*
    A basic color enumeration, used throughout the code below
*/
typedef enum {
    Red, Orange, Yellow, Green, Blue, Indigo, Violet
} color_t;

#define DIM_X 5

typedef color_t colors_one_d_a_t[DIM_X];

colors_one_d_a_t colors_one_d_a =
    {Green, Indigo, Red, Orange, Yellow};

/*
    Note the use of empty square brackets to indicate an array type,
    although it still degenerates to a pointer type.
*/
void print_elements_a( color_t colors_a [] )
{
    printf( "print_elements_a: elements: %d, %d, %d, %d, %d\n",
        colors_a[0], colors_a[1],
        colors_a[2], colors_a[3],
        colors_a[4] );
}

/*
    We can make the pointer type explicit and get the same results.
*/
void print_elements_a_explicit_ptr( color_t * color_p)
{
    printf( "print_elements_a_explicit_ptr: elements: %d, %d, %d, %d, %d\n",
        color_p[0], color_p[1],
        color_p[2], color_p[3],
        color_p[4] );
}

int main(int argc, const char * argv[])
{
    printf( "first 1-D array element: %d\n", colors_one_d_a[0] );

    printf( "address of array: %p, %p, first element: %p\n",
            colors_one_d_a, &colors_one_d_a, &colors_one_d_a[0] );

    printf( "elements direct: %d, %d, %d, %d, %d\n",
            colors_one_d_a[0], colors_one_d_a[1],
            colors_one_d_a[2], colors_one_d_a[3],
            colors_one_d_a[4] );

    color_t * color_p = colors_one_d_a;

    printf( "elements indirect: %d, %d, %d, %d, %d\n",
        color_p[0], color_p[1], color_p[2],
        color_p[3], color_p[4] );

    print_elements_a( colors_one_d_a );
    print_elements_a_explicit_ptr( colors_one_d_a );

    return 0;
}
