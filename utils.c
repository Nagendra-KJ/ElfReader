#include "utils.h"
#include <stdio.h>

void eprintf(int errcode)
{
    fprintf(stderr, "Encountered Error: %d\n", errcode);
}
