#include "../chelper/arraylist.h"
#include "../chelper/string.h"
#include "../chelper/common.h"

int main(void)
{
    ArrayList x = ArrayListCreate(-1, &free, &IntCopy);

    return 0;
}
