
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

#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#if !defined(USE_THREAD_BASED_TIMER)
#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)
#else
#define CPU_TIME (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)
#endif

typedef unsigned int uint;

double heavy_work_0( uint, int );
double heavy_work_1( uint, int );
double heavy_work_2( uint, int );

#if !defined( LARGE_WORK )
#define LARGE_WORK 20000000
#endif

int main( int argc, char **argv )
{
  struct timespec ts;
  double results[3] = {0};

  int N = LARGE_WORK;
  if ( argc > 1 )
    N = (unsigned int) atoi(*(argv+1));
  
  int check_addresses = 0;
  if ( argc > 2 )
    check_addresses = atoi( *(argv+2) );
  else
    printf("\nrun me with a second argument != 0 if you want me to show that\n"
	   " the addresses of the internal variables in heavy_work_*\n"
	   " functions are all different for the different threads\n"
	   " that may be executing the same functions.\n"
	   "i.e., if you give me an argument != 0 I'll show the dynamic\n"
	   " extent of the parallel regions.\n\n");
  
  double tstart = CPU_TIME;
  
#ifdef _OPENMP                                   // ---------- parallel execution
                                                 //  
                                                 //  if N<= 10 the parallel region
                                                 //  will not be created by spawning
                                                 //  the requested number of threads;
                                                 //  only the master thread will
                                                 //  execute the code in the block.
                                                 //  
 #pragma omp parallel if ( N > 10 )
  {
   #pragma omp master    
    printf("running %d threads\n", omp_get_num_threads() );
    
    int myid = omp_get_thread_num();
    int arg  = ( check_addresses? myid : -1 );
    double result;
    
    if( myid % 3 == 0)
      result = heavy_work_0( N, arg );
    else if ( myid % 3 == 1 )
      result = heavy_work_1( N, arg);
    else if ( myid % 3 == 2 )
      result = heavy_work_2( N, arg );

    if ( myid < 3 )
      results[myid] = result;
  }
 #else                                           // ---------- serial execution
						 // 

  results[0] = heavy_work_0( N, 0 );
  results[1] = heavy_work_1( N, 0 );
  results[2] = heavy_work_2( N, 0 );
  
 #endif                                          // ---------------------------
						 // 
  
  double tend = CPU_TIME;
  
  printf("final result is (%g, %g, %g) %g\nachieved in %g seconds\n",
	 results[0], results[1], results[2],
	 results[0] + results[1] + results[2],
	 tend-tstart);
  
 #if !defined(USE_THREAD_BASED_TIMER)
  printf("\nnote: if you notice something strange about the timing, like the fact that\n"
	 "the OpenMP timings is larger than the serial timings, you may consider to use\n"
	 "a different CLOCK, like CLOCK_THREAD_CPUTIME_ID instead of CLOCK_PROCESS_CPUTIME_ID.\n"
	 "To do that, compile this code with -DUSE_THREAD_BASED_TIMER\n");
 #endif
  return 0;
}


double heavy_work_0( uint N, int id )
{
  double guess = 3.141572 / 3;
  if ( id >= 0)
    printf("\t[thread %2d] my guess address in %s is %p\n", id, __func__, &guess);
  
  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }

  return guess;
}

double heavy_work_1( uint N, int id )
{
  double guess = 3.141572 / 3;
  if ( id >= 0)
    printf("\t[thread %2d] my guess address in %s is %p\n", id, __func__, &guess);

  for( int i = 0; i < N; i++ )
    {
      guess = log( guess );
      guess = exp( sqrt(guess)/guess );
    }

  return guess;
}

double heavy_work_2( uint N, int id )
{
  double guess = 3.141572 / 3;
  if ( id >= 0)
    printf("\t[thread %2d] my guess address in %s is %p\n", id, __func__, &guess);

  for( int i = 0; i < N; i++ )
    {
      guess = sqrt( guess );
      guess = exp( 1+1.0/guess );
    }

  return guess;
}
