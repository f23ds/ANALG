/**
 *
 * Descripcion: Implementation of function that generate permutations
 *
 * File: permutations.c
 * Autor: Carlos Aguirre
 * Version: 1.1
 * Fecha: 21-09-2019
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "permutations.h"
/***************************************************/
/* Function: random_num Date:                      */
/* Authors: Ignacio Sánchez and Fabio Desio        */
/*                                                 */
/* Routine that generates a random number           */
/* between two given numbers                       */
/*                                                 */
/* Input:                                          */
/* int inf: lower limit                            */
/* int sup: upper limit                            */
/* Output:                                         */
/* int: random number                              */
/* ERR in case of error                            */
/***************************************************/
int random_num(int inf, int sup)
{
  if (inf < 0 || sup > RAND_MAX || inf > sup)
  {
    return ERR;
  }

  if (inf == sup)
    return sup;

  return inf + (int)(((double)rand() * (sup - inf + 1.0) / (RAND_MAX + 1.0)));
}

/***************************************************/
/* Function: generate_perm Date:                   */
/* Authors: Ignacio Sánchez and Fabio Desio        */
/*                                                 */
/* Routine that generates a random permutation     */
/*                                                 */
/* Input:                                          */
/* int n: number of elements in the permutation    */
/* Output:                                         */
/* int *: pointer to integer array                 */
/* that contains the permutation                   */
/* or NULL in case of error                        */
/***************************************************/
int *generate_perm(int N)
{
  int *perm = NULL;
  int i, tmp, random;

  if (N < 1 || N > RAND_MAX)
    return NULL;

  /* Reservamos memoria para el array de manera dinámica */
  perm = (int *)malloc(N * sizeof(perm[0]));

  if (perm == NULL)
  {
    return NULL;
  }

  for (i = 0; i < N; i++)
  {
    perm[i] = i + 1;
  }

  for (i = 0; i < N; i++)
  {
    random = random_num(i, N-1);
    tmp = perm[i];
    perm[i] = perm[random];
    perm[random] = tmp;
  }

  return perm;
}

/***************************************************/
/* Function: generate_permutations Date:           */
/* Authors: Ignacio Sánchez and Fabio Desio        */
/*                                                 */
/* Function that generates n_perms random          */
/* permutations with N elements                    */
/*                                                 */
/* Input:                                          */
/* int n_perms: Number of permutations             */
/* int N: Number of elements in each permutation   */
/* Output:                                         */
/* int**: Array of pointers to integer that point  */
/* to each of the permutations                     */
/* NULL en case of error                           */
/***************************************************/
int **generate_permutations(int n_perms, int N)
{
  int i, j;
  int **perms = NULL;

  if (n_perms < 1 || n_perms > RAND_MAX || N < 1 || N > RAND_MAX)
    return NULL;

  perms = (int **)malloc(n_perms * sizeof(perms[0]));

  if (perms == NULL)
    return NULL;

  for (i = 0; i < n_perms; i++)
  {
    perms[i] = generate_perm(N);

    if (perms[i] == NULL) {
      for (j = 0; j < i; j++) {
        free(perms[j]);
      }
      free(perms);
      return NULL;
    }
  }

  return perms;
}

/***************************************************/
/* Function: free_permutations Date:               */
/* Authors: Ignacio Sánchez and Fabio Desio        */
/*                                                 */
/* Function that frees the allocated memory for    */
/* a number of permutations                        */
/*                                                 */
/* Input:                                          */
/* int **perms: Matrix of permutations             */
/* int n_perms: Number of permutations             */
/***************************************************/
void free_permutations(int  **perms, int n_perms) {
  int i;

  for (i = 0; i < n_perms; i++) {
    free(perms[i]);
  }

  free(perms);
}