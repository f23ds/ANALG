/**
 *
 * Description: Implementation of functions for search
 *
 * File: search.c
 * Author: Carlos Aguirre and Javier Sanz-Cruzado
 * Version: 1.0
 * Date: 14-11-2016
 *
 */

#include "search.h"
#include "swap.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>

/**
 *  Key generation functions
 *
 *  Description: Receives the number of keys to generate in the n_keys
 *               parameter. The generated keys go from 1 to max. The
 * 				 keys are returned in the keys parameter which must be
 *				 allocated externally to the function.
 */

/**
 *  Function: uniform_key_generator
 *               This function generates all keys from 1 to max in a sequential
 *               manner. If n_keys == max, each key will just be generated once.
 */
void uniform_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for (i = 0; i < n_keys; i++)
    keys[i] = 1 + (i % max);

  return;
}

/**
 *  Function: potential_key_generator
 *               This function generates keys following an approximately
 *               potential distribution. The smaller values are much more
 *               likely than the bigger ones. Value 1 has a 50%
 *               probability, value 2 a 17%, value 3 the 9%, etc.
 */
void potential_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for (i = 0; i < n_keys; i++)
  {
    keys[i] = .5 + max / (1 + max * ((double)rand() / (RAND_MAX)));
  }

  return;
}

PDICT init_dictionary(int size, char order)
{
  PDICT pdict;
  /* Control de errores */
  assert(size > 0);

  /* Reservamos memoria para el diccionario */
  pdict = (PDICT)malloc(sizeof(DICT));
  if (pdict == NULL)
    return NULL;

  pdict->table = (int *)malloc(size * sizeof(pdict->table[0]));
  if (pdict->table == NULL)
  {
    free(pdict);
    return NULL;
  }

  pdict->size = size;
  pdict->n_data = 0;
  pdict->order = order;

  return pdict;
}

void free_dictionary(PDICT pdict)
{
  free(pdict->table);
  free(pdict);
}

int insert_dictionary(PDICT pdict, int key)
{
  int ob = 0, i;
  int *tmp = NULL;
  /* Control de errores */
  assert(pdict != NULL);

  /* Comprobamos si el diccionario está lleno */
  if (pdict->size == pdict->n_data)
  {
    tmp = (int *)realloc(pdict->table, (int)((pdict->size * 1.1) + 1) * sizeof(int));
    if (tmp == NULL)
      return ERR;

    pdict->table = tmp;
  }

  /* Si no está ordenado lo insertamos al final */
  if (pdict->order == NOT_SORTED)
  {
    pdict->table[pdict->n_data] = key;
    pdict->n_data++;
    ob++;
    return ob;
  }

  /* https://www.geeksforgeeks.org/search-insert-and-delete-in-a-sorted-array/ */
  i = pdict->n_data - 1;

  while (i >= 0 && pdict->table[i] > key)
  {
    pdict->table[i + 1] = pdict->table[i];
    i--;
  }

  pdict->table[i + 1] = key;
  pdict->n_data++;

  return ob;
}

int massive_insertion_dictionary(PDICT pdict, int *keys, int n_keys)
{
  int ob = 0, i, st;
  /* Control de errores */
  assert(pdict != NULL);
  assert(keys != NULL);
  assert(n_keys > 0);

  /* No hace falta comprobar que el diccionario esté lleno pues se llama a la función insert_dictionary */
  for (i = 0; i < n_keys; i++)
  {
    st = insert_dictionary(pdict, keys[i]);
    if (st == ERR)
      return ERR;
    ob += st;
  }

  return ob;
}

int search_dictionary(PDICT pdict, int key, int *ppos, pfunc_search method)
{
  int ob = 0, st;
  /* Control de errores */
  assert(pdict != NULL);
  assert(ppos != NULL);
  assert(method != NULL);

  st = method(pdict->table, 0, pdict->n_data, key, ppos);
  if (st == NOT_FOUND)
    return ERR;

  ob += st;

  return ob;
}

/* Search functions of the Dictionary ADT */
int bin_search(int *table, int F, int L, int key, int *ppos)
{
  int M, ob = 0;

  /* Control de errores */
  assert(table != NULL);
  assert(F <= L);
  assert(ppos != NULL);

  while (F <= L)
  {
    M = (F + L) / 2;
    if (++ob && table[M] == key)
    {
      *ppos = M;
      return ob;
    }
    else if (++ob && key < table[M])
    {
      L = M - 1;
    }
    else
    {
      F = M + 1;
    }
  }

  *ppos = NOT_FOUND;
  return ERR;
}

int lin_search(int *table, int F, int L, int key, int *ppos)
{
  int ob = 0;

  /* Control de errores */
  assert(table != NULL);
  assert(F <= L);
  assert(ppos != NULL);

  for (; F <= L; F++)
  {
    if (++ob && table[F] == key)
    {
      *ppos = F;
      return ob;
    }
  }

  *ppos = NOT_FOUND;
  return ERR;
}

int lin_auto_search(int *table, int F, int L, int key, int *ppos)
{
  int ob = 0, i;

  /* Control de errores */
  assert(table != NULL);
  assert(F <= L);
  assert(ppos != NULL);

  /* Caso de encontrar la clave en la primera posición */
  if (++ob && table[F] == key)
  {
    *ppos = F;
    return ob;
  }

  /* Caso general */
  for (i = 1; i <= L; i++)
  {
    if (++ob && table[i] == key)
    {
      *ppos = i;
      /* Intercambiamos con la posición anterior */
      swap(&table[i], &table[i - 1]);
      return ob;
    }
  }

  *ppos = NOT_FOUND;
  return ERR;
}