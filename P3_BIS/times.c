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
#include <assert.h>
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
    if (begin == (clock_t)-1)
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
    if (end == (clock_t)-1)
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
      fclose(pf);
      return ERR;
    }
  }

  fclose(pf);

  return OK;
}

short generate_search_times(pfunc_search method, pfunc_key_generator generator,
                            int order, char *file,
                            int num_min, int num_max,
                            int incr, int n_times)
{
  TIME_AA *time = NULL;
  int i, j, N;
  short status = ERR;

  /* Control de errores inicial */
  assert(method != NULL);
  assert(generator != NULL);
  assert(file != NULL);
  assert(num_min <= num_max);

  /* Guardamos memoria para los tiempos de ejecución */
  time = (PTIME_AA)malloc(n_times * sizeof(time[0]));
  if (time == NULL)
    return ERR;

  N = num_max - num_min;

  /* Generamos en este array todos los tiempos medios y los imprimimos */
  for (i = 0, j = num_min; i < N * n_times && j <= num_max; i++, j += incr)
  {
    status = average_search_time(method, generator, order, N, n_times, &time[i]);
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

short average_search_time(pfunc_search metodo, pfunc_key_generator generator,
                          int order,
                          int N,
                          int n_times,
                          PTIME_AA ptime)
{
  PDICT pdict = NULL;
  int *perm = NULL, st, n_keys, *tsearch = NULL, i, min_ob = INT_MAX, max_ob = 0, ob, pos = 0;
  long suma_obs = 0;
  double tiempo;
  long double suma_tiempo = 0;
  clock_t begin, end;

  /* Control de errores */
  assert(metodo != NULL);
  assert(generator != NULL);
  assert(ptime != NULL);
  assert(n_times > 0 && N > 0);

  /* Creamos un diccionario de tamaño N */
  pdict = init_dictionary(N, order);
  if (pdict == NULL)
    return ERR;

  /* Creamos una permutación de tamaño N */
  perm = generate_perm(N);
  if (perm == NULL)
  {
    free_dictionary(pdict);
    return ERR;
  }

  /* Insertamos los elementos de la permutación */
  st = massive_insertion_dictionary(pdict, perm, N);
  if (st == ERR)
  {
    free_dictionary(pdict);
    free(perm);
    return ERR;
  }

  /* Reservamos memoria para la tabla de las n_times * N claves a buscar */
  n_keys = n_times * N;
  tsearch = (int *)malloc(n_keys * sizeof(tsearch[0]));
  if (tsearch == NULL)
  {
    free_dictionary(pdict);
    free(perm);
    return ERR;
  }

  /* Llenamos la tabla anterior con los generadores de claves */
  generator(tsearch, n_keys, N);

  /* Medimos el tiempo que tarda en buscar las claves */
  for (i = 0; i < n_keys; i++)
  {
    begin = clock();
    if (begin == (clock_t)-1)
    {
      free_dictionary(pdict);
      free(perm);
      free(tsearch);
      return ERR;
    }

    ob = metodo(pdict->table, 0, n_keys, tsearch[i], &pos);
    if (ob == ERR)
    {
      free_dictionary(pdict);
      free(perm);
      free(tsearch);
      return ERR;
    }

    end = clock();
    if (end == (clock_t)-1)
    {
      free_dictionary(pdict);
      free(perm);
      free(tsearch);
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
  ptime->N = N;
  ptime->n_elems = n_keys;
  ptime->time = suma_tiempo / (double)n_keys;
  ptime->average_ob = suma_obs / (double)n_keys;
  ptime->min_ob = min_ob;
  ptime->max_ob = max_ob;

  return OK;
}
