#include <stdio.h>

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

colors_two_d_a_t colors_two_d_a = {
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
    /*
        Uncomment to demonstrate a compiler error indicating
        that an argument of type "int" was expected, but
        instead a pointer to type color_t was passed. This
        illustrates how the row type degenerates to the
        type of its elements when passed as a parameter.
    */
    // printf( "elements: %d, %d, %d, %d, %d\n",
    //     colors_two_d_a[0], colors_two_d_a[1],
    //     colors_two_d_a[2], colors_two_d_a[3],
    //     colors_two_d_a[4] );

    /*
        This accesses an array element that is out of bounds,
        but your compiler may not generate a warning.
    */
    // printf( "elements: %d, %d, %d, %d\n",
    //     colors_two_d_a[0][0], colors_two_d_a[1][1],
    //     colors_two_d_a[5][3], colors_two_d_a[6][5] );

    /*
        This should fix the out-of-bounds warning
    */
    // printf( "elements: %d, %d, %d, %d\n",
    //     colors_two_d_a[0][0], colors_two_d_a[1][1],
    //     colors_two_d_a[5][3], colors_two_d_a[6][4] );

    /*
        This line should generate an error, since it
        attempts to assign a degenerated array type,
        which will have the type of pointer to the
        array row type, to a variable of pointer to
        the row element type.
    */
    // color_t * color_two_p = colors_two_d_a;

    /*
        This should compile, as the array row type on
        the left matches the degenerated array type.
    */
    colors_one_d_a_t * color_one_d_a_p = colors_two_d_a;

    /*
        This code looks like the code above, but will
        access elements using a pointer adjusted by offsets
        at runtime, instead of via element addresses calculated
        at compile time.
    */
    printf( "elements: %d, %d, %d, %d\n",
        color_one_d_a_p[0][0], color_one_d_a_p[1][1],
        color_one_d_a_p[5][3], color_one_d_a_p[6][4] );

    return 0;
}
