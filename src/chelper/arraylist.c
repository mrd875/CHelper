#include <stdlib.h>

typedef struct ArrayList
{
    void **arr;
    size_t length;
    size_t size;

} * ArrayList;