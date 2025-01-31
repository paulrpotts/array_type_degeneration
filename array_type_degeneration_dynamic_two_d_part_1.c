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

    // This should not compile, as objects can't be cast to array
    // types.

    // colors_two_d_a_t colors_two_d_a = (colors_two_d_a_t)raw_mem_p;

    // This should compile, but it is not very useful, as the type
    // is a pointer to a whole 2-D array type, not a row or element
    // type.

    // colors_two_d_a_t * colors_two_d_p = (colors_two_d_a_t *)raw_mem_p;

    // This should not compile, as we're not allowed to assign to
    // an array.

    // *colors_two_d_p = colors_two_d_a;

    // This also should not compile, as we simply can't assign
    // a pointer type on the right to an array type like this,
    // but note that even if we could, the left hand side would
    // refer to rows of a nonexistent second two-dimensional
    // array past the end of our allocated memory.

    // for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    // {
    //     colors_two_d_p[y_idx] = colors_two_d_a[y_idx];
    // }

    // This also fails, because we still have a pointer to the
    // whole array on the left.

    // for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    // {
    //     for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
    //     {
    //         colors_two_d_p[y_idx][x_idx] =
    //             colors_two_d_a[y_idx][x_idx];
    //     }
    // }

    // These lines demonstrate that the stride being applied is the length
    // our whole array-of-arrays:

    // colors_two_d_a_t * colors_two_d_p = (colors_two_d_a_t *)raw_mem_p;
    // void * array_of_arrays_1_p = colors_two_d_p[0];
    // void * array_of_arrays_2_p = colors_two_d_p[1];
    // printf("diff: %td\n", array_of_arrays_2_p - array_of_arrays_1_p);

    // This approach also will not work:

    // colors_two_d_a_t * colors_two_d_p = (colors_two_d_a_t *)raw_mem_p;
    // for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    // {
    //     colors_one_d_a_t * colors_one_d_p = colors_two_d_p[y_idx];
    
    //     for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
    //     {
    //         color_t * color_p = colors_one_d_p[x_idx];
    //         *color_p = colors_two_d_a[y_idx][x_idx];
    //     }
    // }

    // If we make an instrumented version which tracks the difference
    // between generated pointers, but does not write to memory, we can
    // see what is going wrong.

    // colors_two_d_a_t * colors_two_d_p = (colors_two_d_a_t *)raw_mem_p;
    // void * colors_one_d_prev_p = &colors_two_d_p[0];

    // for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    // {
    //     colors_one_d_a_t * colors_one_d_p = colors_two_d_p[y_idx];
    //     printf("diff of row pointer: %td\n",
    //         (void *)colors_one_d_p - colors_one_d_prev_p);
    //     colors_one_d_prev_p = colors_one_d_p;
    //     void * color_prev_p = &colors_one_d_p[0];
    //     for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
    //     {
    //         color_t *color_p = colors_one_d_p[x_idx];
    //         printf("diff of element pointer: %td\n",
    //             (void *)color_p - color_prev_p);
    //         color_prev_p = color_p;
    //     }
    // }

    // Using explicit pointer math with our destination will work,
    // though. We can create a pointer to the element type and
    // apply the array math explicitly, either with or without
    // using square bracket notation.

    color_t * colors_two_d_p = (color_t *)raw_mem_p;

    for ( unsigned int y_idx = 0; y_idx < DIM_Y; y_idx++ )
    {
        for ( unsigned int x_idx = 0; x_idx < DIM_X; x_idx++ )
        {
            // Create the element offset explicitly
            colors_two_d_p[y_idx * DIM_X + x_idx] = 
                colors_two_d_a[y_idx][x_idx];

            // We can also do it without the square brackets
            // on the left side:
            // *( colors_two_d_p + y_idx * DIM_X + x_idx ) = 
            //     colors_two_d_a[y_idx][x_idx];

            // We can't use the same approach on the right-hand
            // side, though. This does not work:
            // *( colors_two_d_p + y_idx * DIM_X + x_idx ) = 
            //     *( colors_two_d_a + y_idx * DIM_X + x_idx );
            // The right-hand side pointer is incrementing by
            // 20 bytes, not 4. This is because the array type
            // degenerates to the type of a pointer to its first
            // element, and that type is a row, not an element:
            // printf("right-hand-side pointer: %p\n",
            //     (void *)( colors_two_d_a + y_idx * DIM_X + x_idx ));
            // printf("Diff from row start address: %td\n",
            //     (void *)( colors_two_d_a + y_idx * DIM_X + x_idx )
            //         - (void *)( colors_two_d_a[y_idx] ) );

            // This will work, though, although the extra
            // dereference on the right side makes it look
            // wrong:
            // *( colors_two_d_p + y_idx * DIM_X + x_idx ) = 
            //     *( *colors_two_d_a + y_idx * DIM_X );
            // printf("right-hand-side pointer: %p\n",
            //     (void *)( *colors_two_d_a + y_idx * DIM_X + x_idx ));
            // printf("Diff from row start address: %td\n",
            //     (void *)( *colors_two_d_a + y_idx * DIM_X + x_idx )
            //         - (void *)( colors_two_d_a[y_idx] ) );

            // We also could set up an explicit pointer, which
            // would allow us to use similar syntax on each side:
            // color_t * source_color_a_p = *colors_two_d_a;
            // Note that the above definition could be moved above
            // the loops since it does not vary.
            // *( colors_two_d_p + y_idx * DIM_X + x_idx ) = 
            //     *( source_color_a_p + y_idx * DIM_X );
            // printf("right-hand-side pointer: %p\n",
            //     (void *)( source_color_a_p + y_idx * DIM_X + x_idx ));
            // printf("Diff from row start address: %td\n",
            //     (void *)( source_color_a_p + y_idx * DIM_X + x_idx )
            //         - (void *)( colors_two_d_a[y_idx] ) );

            // Note that none of these versions of the assignment
            // expression are as simple and elegant as we might hope.
        }
    }

    printf( "elements: %d, %d, %d, %d\n",
        *colors_two_d_p,
        *(colors_two_d_p + DIM_X * 1 + 1),
        *(colors_two_d_p + DIM_X * 5 + 3),
        *(colors_two_d_p + DIM_X * 6 + 4));

    return 0;
}
