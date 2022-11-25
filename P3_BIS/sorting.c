/**
 *
 * Descripcion: Implementation of sorting functions
 *
 * Fichero: sorting.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include <stdlib.h>
#include <assert.h>
#include "sorting.h"
#include "swap.h"

/********************************************************/
/* Function: SelectSort Date:                           */
/* Authors: Ignacio Sánchez and Fabio Desio             */
/*                                                      */
/* Function that sorts a disordered integer table       */
/* ascendantly                                          */
/*                                                      */
/* Input:                                               */
/* int *array: integer table                            */
/* int ip: first index of the array                     */
/* int iu: last index of the array                      */
/* Output:                                              */
/* int ob: number of ob performed during the ordenation */
/* ERR in case of error                                 */
/********************************************************/
int SelectSort(int *array, int ip, int iu)
{
  int i, ob = 0, minimum;

  if (array == NULL || ip < 0 || iu < ip)
  {
    return ERR;
  }

  for (i = ip; i < iu; i++)
  {
    minimum = min(array, i, iu, &ob);
    swap(array + i, array + minimum);
  }

  return ob;
}

/*****************************************************/
/* Function: SelectSortInv Date:                     */
/* Authors: Ignacio Sánchez and Fabio Desio          */
/*                                                   */
/* Function that sorts a disordered integer table    */
/* in descending order                               */
/*                                                   */
/* Input:                                            */
/* int *array: integer table                         */
/* int ip: first index of the array                  */
/* int iu: last index of the array                   */
/* Output:                                           */
/* int ob: number of ob performed during the sorting */
/* ERR in case of error                              */
/*****************************************************/
int SelectSortInv(int *array, int ip, int iu)
{
  int i, ob = 0, minimum;

  if (array == NULL || ip < 0 || iu < ip)
  {
    return ERR;
  }

  for (i = iu; i > 0; i--)
  {
    minimum = min(array, ip, i, &ob);
    swap(array + i, array + minimum);
  }

  return ob;
}

/**************************************************/
/* Function: min Date:                            */
/* Authors: Ignacio Sánchez and Fabio Desio       */
/*                                                */
/* Function that saves the index of the smallest  */
/* element between the elements corresponding     */
/* to the indexes ip and iu                       */
/*                                                */
/* Input:                                         */
/* int *array: table of integers                  */
/* int ip: first index of the array               */
/* int iu: last index of the array                */
/* int *ob: pointer that saves the number of      */
/* comparisons made                               */
/* Output:                                        */
/* int min: index of the smallest element         */
/* ERR in case of error                           */
/**************************************************/
int min(int *array, int ip, int iu, int *ob)
{
  int i, min;

  if (array == NULL || ip < 0 || iu < ip)
  {
    return ERR;
  }

  min = ip;

  for (i = ip; i <= iu; i++)
  {
    (*ob)++;
    if (array[i] < array[min])
    {
      min = i;
    }
  }

  return min;
}

/**
 * Recursive sorting algorithm of an array of integers that divides the array 
 * into two smaller arrays and sorts them from bottom to top using the merge routine
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @return Number of basic operations performed by the algorithm
 */
int mergesort(int *tabla, int ip, int iu)
{
  int ob = 0;
  int imedio;
  /* Control de errores */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);

  /* Caso base */
  if (iu == ip)
    return OK;

  imedio = (iu + ip) / 2;

  /* Separamos en dos mitades recursivas */
  ob += mergesort(tabla, ip, imedio);
  ob += mergesort(tabla, imedio + 1, iu);

  /* Hacemos merge */
  ob += merge(tabla, ip, iu, imedio);

  return ob;
}

/**
 * Routine related to the Mergesort algorithm that sorts an array of integers 
 * dividing it into two smaller arrays and comparing them
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @param imedio medium index of the array
 * @return Number of basic operations performed by the algorithm
 */
int merge(int *tabla, int ip, int iu, int imedio)
{
  int *aux = NULL, size, i, j, k, ob = 0;
  /* Control de errores */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);

  size = iu - ip + 1;

  /* Reservamos memoria para la tabla auxiliar de índices */
  aux = (int *)malloc(sizeof(aux[0]) * size);
  /* Hacemos assert pues no debemos liberar nada en caso de error */
  assert(aux != NULL);

  for (i = ip, j = imedio + 1, k = 0; i <= imedio && j <= iu && k < size; k++)
  {
    /* Comparación de claves */
    if (++ob && tabla[i] < tabla[j])
    {
      aux[k] = tabla[i];
      i++;
    }
    else
    {
      aux[k] = tabla[j];
      j++;
    }
  }

  /* Copiamos resto de la tabla derecha */
  if (i > imedio)
  {
    while (++ob && j <= iu)
    {
      aux[k] = tabla[j];
      j++;
      k++;
    }
  } /* Copiamos el resto de la tabla izquierda */
  else if (j > iu)
  {
    while (++ob && i <= imedio)
    {
      aux[k] = tabla[i];
      i++;
      k++;
    }
  }

  for (k = 0; k < size; k++)
  {
    tabla[ip + k] = aux[k];
  }

  free(aux);

  return ob;
}

/**
 * Recursive algorithm that sorts an array of integers 
 * using the partition subroutine
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @return Number of basic operations performed by the algorithm
 */
int quicksort(int *tabla, int ip, int iu)
{
  int pos, ob = 0;

  /* Control de errores */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);

  /* CASO BASE */
  if (iu == ip)
    return OK;

  /* Realizamos la partición */
  ob += partition(tabla, ip, iu, &pos);

  /*Recursión*/
  if (ip < pos - 1)
    ob += quicksort(tabla, ip, pos - 1);

  if (pos + 1 < iu)
    ob += quicksort(tabla, pos + 1, iu);

  return ob;
}

/**
 * Routine performed by quicksort that takes an element of the array 
 * as the pivot using another routine and leaves the smaller elements 
 * on its left and the greater ones on its right
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @param pos pointer to the index of the element used as pivot
 * @return Number of basic operations performed by the algorithm
 */
int partition(int *tabla, int ip, int iu, int *pos)
{
  int ele, i, ob = 0;
  /* CONTROL DE ERRORES */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);
  assert(pos != NULL);

  ob += median(tabla, ip, iu, pos);

  ele = tabla[*pos];
  /* Realizamos un primer swap */
  swap(&tabla[ip], &ele);

  *pos = ip;

  for (i = ip + 1; i <= iu; i++)
  {
    /* Comparación de claves */
    if (++ob && tabla[i] < ele)
    {
      (*pos)++;
      swap(&tabla[i], &tabla[*pos]);
    }
  }

  swap(&tabla[ip], &tabla[*pos]);

  return ob;
}

/**
 * Function that assings the index of the first element to the pivot
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @param pos pointer to the index of the element used as pivot
 * @return Number of basic operations performed by the algorithm
 */
int median(int *tabla, int ip, int iu, int *pos)
{
  /* CONTROL DE ERRORES */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);
  assert(pos != NULL);

  *pos = ip;

  return OK;
}

/**
 * Function that assings the floor of the average value between 
 * the first and last indexes to the pivot
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @param pos pointer to the index of the element used as pivot
 * @return Number of basic operations performed by the algorithm
 */
int median_avg(int *tabla, int ip, int iu, int *pos)
{
  /* CONTROL DE ERRORES */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);
  assert(pos != NULL);

  *pos = (ip + iu) / 2;

  return OK;
}

/**
 * Function that assings the index of the intermediate element between 
 * the first, the last and the medium element of the array to the pivot
 * @param tabla pointer to an array of integers
 * @param ip first index of the array
 * @param iu last index of the array
 * @param pos pointer to the index of the element used as pivot
 * @return Number of basic operations performed by the algorithm
 */
int median_stat(int *tabla, int ip, int iu, int *pos)
{
  int e1, e2, e3, im, ob = 0;
  /* CONTROL DE ERRORES */
  assert(tabla != NULL);
  assert(ip >= 0);
  assert(iu >= ip);
  assert(pos != NULL);

  im = (ip + iu) / 2;
  e1 = tabla[ip];
  e2 = tabla[iu];
  e3 = tabla[im];

  /* Algoritmo de comparación de tres elementos */
  if (++ob && e1 < e2)
  {
    if (++ob && e2 <= e3)
    {
      *pos = iu;
    }
    else if (++ob && e1 < e3)
    {
      *pos = im;
    }
    else
    {
      *pos = ip;
    }
  }
  else
  {
    if (++ob && e2 >= e3)
    {
      *pos = iu;
    }
    else if (++ob && e1 > e3)
    {
      *pos = im;
    }
    else
    {
      *pos = ip;
    }
  }

  return ob;
}