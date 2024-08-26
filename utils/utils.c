#include "utils.h"
#include <stdio.h>

void eprintf(int errcode)
{
    fprintf(stderr, "Encountered Error: %d\n", errcode);
}

void print_star()
{
    for (int i = 0; i < NUM_STAR; ++i)
        printf("*");
    printf("\n");
}
