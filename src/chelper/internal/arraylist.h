#ifndef __CHELPARRAYLISTINTERN__
#define __CHELPARRAYLISTINTERN__

#include "../arraylist.h"
#include "../common.h"

/*Clears the free'ing of the list. Used for when you want to move items. */
void ArrayListClearFree(ArrayList l);

/*Gets the real array.*/
void **ArrayListGetArray(ArrayList l);

/*Gets the free function pointer*/
free_fn_t ArrayListGetFreeFn(ArrayList l);

/*Gets the copy func ptr*/
copy_fn_t ArrayListGetCopyFn(ArrayList l);

#endif /* __CHELPARRAYLISTINTERN__ */
