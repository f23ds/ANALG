#include "swap.h"
#include <stdlib.h>

/******************************************************* */
/* Function: swap Date:                                  */
/* Authors: Ignacio Sánchez and Fabio Desio              */
/*                                                       */
/* Function that swaps two elements of an interger table */
/*                                                       */
/* Input:                                                */
/* int *p1: element of the integer table                 */
/* int *p2: element of the integer table                 */
/* int iu: last index of the array                       */
/******************************************************* */
void swap(int *p1, int *p2)
{
    int tmp = 0;

    tmp = (*p1);
    (*p1) = (*p2);
    (*p2) = tmp;
}