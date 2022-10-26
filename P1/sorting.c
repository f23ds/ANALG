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

/* TODO: CABECERA */
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
  if (ob == ERR)
    return ERR;

  return ob;
}

/* TODO: CABECERA */
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
    while (j <= iu)
    {
      ob++;
      aux[k] = tabla[j];
      j++;
      k++;
    }
  } /* Copiamos el resto de la tabla izquierda */
  else if (j > iu)
  {
    while (i <= imedio)
    {
      ob++;
      aux[k] = tabla[i];
      i++;
      k++;
    }
  }

  /* Copiamos la tabla auxiliar entre los índices ip e iu */
  for (k = 0; k < size; k++)
  {
    swap(&aux[k], &tabla[ip + k]);
  }

  free(aux);

  return ob;
}