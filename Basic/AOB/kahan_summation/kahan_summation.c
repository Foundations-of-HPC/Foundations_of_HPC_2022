


/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */

/*
 * This example demonstrate how the floating point
 * arithmetics is *not* associative.
 * A float array is filled with random numbers and the
 * summed up.
 *
 * normal_summation  : straightforward summation
 * normal_dsummation : like the previous one, but the
 *                     accumulator is a long double
 * kahan_summation   : implements the Kahan's algorithm
 *                     to mitigate loss of precision
 *
 *  
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>



float normal_summation(float *array, int N, int R)
//
// data are float
// accumulator is float too
//
{
  int i;
  float SUM = 0;

  if(!R)
#pragma GCC ivdep
    for (i = 0; i < N; i++)
      SUM += array[i];
  else
#pragma GCC ivdep    
    for (i = N-1; i >= 0; i--)
      SUM += array[i];

  return SUM;
}

float normal_dsummation(float *array, int N, int R)
//
// data are float
// accumulator is long double
//
{
  int i;
  long double SUM = 0;

  if(!R)
 #pragma GCC ivdep
    for (i = 0; i < N; i++)
      SUM += array[i];
  else
 #pragma GCC ivdep
    for (i = N-1; i >= 0; i--)
      SUM += array[i];
    
  return (float)SUM;
}


float kahan_summation(float *array, int N, int R)
//
// data are float
// accumulator is float too!
//
{
  int i;
  float SUM = 0;
  float c, t, y;
  c = 0;

  if(!R)
#pragma GCC ivdep  
    for (i = 0; i < N; i++)
      {
	y = array[i] - c;
	t = SUM + y;
	c = (t - SUM) -y;
	SUM = t;
      }
  else
#pragma GCC ivdep  
    for (i = N-1; i >= 0; i--)
      {
	y = array[i] - c;
	t = SUM + y;
	c = (t - SUM) -y;
	SUM = t;
      }
    

  return SUM;
}



int compare(const void *A, const void *B)
{
  float a = *(float*)A;
  float b = *(float*)B;

  return (a > b) - (a < b);
}


int main(int argc, char **argv)
{
  int    N, i;
  float *array, max;

  if ( argc > 1 )
    {
      N     = atoi(*(argv + 1));
      if (argc == 3)
	max = atof(*(argv + 2));
      else
	max = 2;
    }
  else
    N = 1000000;
  
  array = (float*)malloc(sizeof(float) * N);

  srand48(clock());

  for (i = 0; i < N; i++)
    {
      array[i] = (float)(drand48() * max);
      if(drand48() < 0.3)
	array[i] /= max;
    }


  FILE *file;
  file = fopen("kahan_numbers.dat", "w");
  for(i = 0; i < N; i++)
    fprintf(file, "%g\n", array[i]);
  fclose(file);
  
  float normal, dnormal, kahan;
  double reference;
  
  // ------------------------------------
  
  normal  = normal_summation(array, N, 0);
  reference = normal;
  dnormal = normal_dsummation(array, N, 0);
  kahan   = kahan_summation(array, N, 0);
  
  printf("summation of random array:\n");
  printf("\nstd: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n",
  	 normal, dnormal, kahan);
  printf("%17s\t dstd: %12.9g\t kahan: %12.9g\n\n", " ",
  	 (normal - dnormal) / normal, (normal - kahan)/normal);


  // ------------------------------------

  qsort(array, N, sizeof(float), compare);
  
  normal  = normal_summation(array, N, 0);
  dnormal = normal_dsummation(array, N, 0);
  kahan   = kahan_summation(array, N, 0);

  printf("summation of sorted random array:\n");
  printf("\nstd: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n",
  	 normal, dnormal, kahan);
  printf("std: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n\n",
  	 (normal- reference) / reference, (normal - dnormal) / normal, (normal - kahan)/normal);


  // ------------------------------------
  
  normal  = normal_summation(array, N, 1);
  dnormal = normal_dsummation(array, N, 1);
  kahan   = kahan_summation(array, N, 1);

  printf("reverse summation of sorted random array:\n");
  printf("\nstd: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n",
  	 normal, dnormal, kahan);
  printf("std: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n\n", 
  	 (normal- reference) / reference, (normal - dnormal) / normal, (normal - kahan)/normal);


  // ------------------------------------
  
  for (i = N; i > 0; i--)
    array[N-i] = (float)((double)1.0/(double)i);
  
  normal  = normal_summation(array, N, 0);
  reference = normal;
  dnormal = normal_dsummation(array, N, 0);
  kahan   = kahan_summation(array, N, 0);

  
  
  printf("reverse 1/n^2 summation:\n");
  printf("std: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n",
  	 normal, dnormal, kahan);
  printf("%17s\t dstd: %12.9g\t kahan: %12.9g\n\n", " ",
  	 (normal - dnormal) / normal, (normal - kahan)/normal);

  
  // ------------------------------------
  
  normal  = normal_summation(array, N, 1);
  dnormal = normal_dsummation(array, N, 1);
  kahan   = kahan_summation(array, N, 1);

  printf("1/n^2 summation:\n");
  printf("std: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n",
  	 normal, dnormal, kahan);
  printf("std: %12.9g\t dstd: %12.9g\t kahan: %12.9g\n\n",
  	 (normal- reference) / reference, (normal - dnormal) / normal, (normal - kahan)/normal);

  return 0;
}
