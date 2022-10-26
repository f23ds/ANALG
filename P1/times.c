/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "times.h"
#include "sorting.h"
#include "permutations.h"

/*******************************************************/
/* Function: average_sorting_time Date:                */
/*                                                     */
/* Function that sorts a number of permutations        */
/* with a method and stores time spent and number      */
/* of ob in a structure                                */
/*                                                     */
/* Input:                                              */
/* pfunc_sort metodo: pointer to the method that sorts */
/* the permutation                                     */
/* int n_perms: Number of permutations to sort         */
/* int N: Number of elements of each permutation       */
/* PTIME_AA ptime: pointer to the struct time_aa       */
/* Output:                                             */
/* OK                                                  */
/* ERR in case of error                                */
/*******************************************************/
short average_sorting_time(pfunc_sort metodo, int n_perms, int N, PTIME_AA ptime)
{
  int i;
  int ob, min_ob = INT_MAX, max_ob = 0;
  long suma_obs = 0;
  int **perms = NULL;
  double tiempo;
  long double suma_tiempo = 0;
  clock_t begin, end;

  /* Control de errores */
  if (metodo == NULL || n_perms < 1 || N < 1 || ptime == NULL)
    return ERR;

  /* Rellenamos los dos primeros campos de la estructura time */
  ptime->N = N;
  ptime->n_elems = n_perms;

  /* Generamos las permutaciones */
  perms = generate_permutations(n_perms, N);

  if (perms == NULL)
    return ERR;

  /* Ordenamos cada permutación con un bucle desde la pos 0 a n_perms */
  for (i = 0; i < n_perms; i++)
  {
    begin = clock();
    if (begin == -1)
    {
      free_permutations(perms, n_perms);
      return ERR;
    }

    ob = metodo(perms[i], 0, N - 1);
    if (ob == ERR)
    {
      free_permutations(perms, n_perms);
      return ERR;
    }

    end = clock();
    if (end == -1)
    {
      free_permutations(perms, n_perms);
      return ERR;
    }

    /* Almacenamos las obs */
    suma_obs += ob;

    /* Almacenamos valor mínimo y máximo */
    if (min_ob > ob)
      min_ob = ob;

    if (max_ob < ob)
      max_ob = ob;

    /* Almacenamos los tiempos */
    tiempo = (double)(end - begin) / CLOCKS_PER_SEC * 1e9; /* Multiplicamos por 1e9 para extraer el tiempo en nanosegundos */
    suma_tiempo += tiempo;
  }

  /* Almacenamos los campos que faltan */
  ptime->time = suma_tiempo / (double)n_perms;
  ptime->average_ob = suma_obs / (double)n_perms;
  ptime->min_ob = min_ob;
  ptime->max_ob = max_ob;

  free_permutations(perms, n_perms);
  return OK;
}

/********************************************************/
/* Function: generate_sorting_times Date:               */
/*                                                      */
/* Function that calls average_sorting_time to sort a   */
/* number of permutations that increments its size      */
/* according to a parameter and prints the sorting      */
/* times in a file                                      */
/*                                                      */
/* Input:                                               */
/* pfunc_sort metodo: pointer to the method that sorts  */
/* the permutation                                      */
/* char *file: pointer to the name of the file          */
/* int num_min: minimum size of the permutations        */
/* int num_max: maximum size of the permutations        */
/* int incr: increment to the size of the permutation   */
/* int n_perms: Number of permutations to sort          */
/* Output:                                              */
/* OK                                                   */
/* ERR in case of error                                 */
/********************************************************/
short generate_sorting_times(pfunc_sort method, char *file, int num_min, int num_max,
                             int incr, int n_perms)
{
  TIME_AA *time = NULL;
  int i, j, n_times;
  short status = ERR;

  /* Control de errores inicial */
  if (method == NULL || file == NULL || num_min < 1 || num_min > num_max || n_perms < 1)
    return ERR;

  /* TODO: Comentar esto en la memoria */
  n_times = ((num_max - num_min) / incr) + 1;
  /* Guardamos memoria para los tiempos de ejecución */
  time = (TIME_AA *)malloc(n_times * sizeof(time[0]));

  if (time == NULL)
    return ERR;

  /* Generamos en este array todos los tiempos medios y los imprimimos */
  for (i = 0, j = num_min; i < n_perms && j <= num_max; i++, j += incr)
  {
    status = average_sorting_time(method, n_perms, j, &time[i]);

    if (status == ERR)
    {
      free(time);
      return ERR;
    }
  }

  status = save_time_table(file, time, n_times);

  if (status == ERR)
  {
    free(time);
    return ERR;
  }

  free(time);
  return OK;
}

/********************************************************/
/* Function: save_time_table Date:                      */
/*                                                      */
/* Function that prints the elements of the struct for  */
/* each size of permutation in a file                   */
/*                                                      */
/* Input:                                               */
/* char *file: pointer to the name of the file          */
/* PTIME_AA ptime: pointer to the struct time_aa        */
/* int n_times: number of times that the increment is   */
/* realised                                             */
/* Output:                                              */
/* OK                                                   */
/* ERR in case of error                                 */
/********************************************************/
short save_time_table(char *file, PTIME_AA ptime, int n_times)
{
  FILE *pf;
  int i;

  if (ptime == NULL)
    return ERR;

  pf = fopen(file, "w");

  if (pf == NULL)
    return ERR;

  for (i = 0; i < n_times; i++)
  {
    if (fprintf(pf, "%d %.2f %.2f %d %d\n", ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].min_ob, ptime[i].max_ob) < 0)
    {
      return ERR;
    }
  }

  fclose(pf);

  return OK;
}
