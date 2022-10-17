/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 *     This code is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License 
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 */



#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 201112L)    // c11
#    define _XOPEN_SOURCE 700
#  elif (__STDC_VERSION__ >= 199901L)  // c99
#    define _XOPEN_SOURCE 600
#  else
#    define _XOPEN_SOURCE 500          // c90
#  endif
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if _XOPEN_SOURCE >= 600
#  include <strings.h>
#endif
#include <time.h>
#include <math.h>
//#include <sys/resource.h>


//  ================================================

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), \
		  (double)ts.tv_sec +				  \
		  (double)ts.tv_nsec * 1e-9)


#define N_DEFAULT 4000
#define ITER      100


//  ================================================

int    access_the_stack   ( void );

int    access_the_stack_dp( void );

int    access_the_heap    ( void );

int    access_the_heap_dp ( void );



//  ================================================

float *on_heap;                                         // this is a global variable

//  ================================================

int access_the_stack( void )
{
  float  on_stack[N_DEFAULT];
  double time     = 0;
  double std_dev  = 0;
  struct  timespec ts;
  
  for ( int r = 0; r < ITER; r++ )
    {
      double t0 = CPU_TIME;
      
      for ( int i = 0; i < N_DEFAULT; i++ )
	on_stack[ i ] = (float)i;
      float volatile access = 0;
      for ( int i = 0; i < N_DEFAULT; i++ )
	access += on_stack[ i ] + 1.0;

      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }
  
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);
  
  printf("timing for accessing the stack was: %g (sigma: %g)\n\n", time, std_dev);

  return 0;
}


int access_the_stack_dp( void )
{
  float  on_stack[N_DEFAULT];

  double time     = 0;
  double std_dev  = 0;
  struct  timespec ts;

    for ( int r = 0; r < ITER; r++ )
    {
      float  register * ptr;
      const float  register * ptr_end = on_stack + N_DEFAULT;
      double t0 = CPU_TIME;

      for ( ptr = on_stack ; ++ptr < ptr_end; )
	*ptr = (float)(ptr-on_stack);
      float volatile access = 0;      
      for ( ptr = on_stack; ++ptr < ptr_end; )
	access += *ptr + 1.0;

      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }
  
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);
  
  printf("timing for accessing the stack was: %g (sigma: %g)\n\n", time, std_dev);

  return 0;
}


int access_the_heap( void )
{
  double time    = 0;
  double std_dev = 0;
  struct  timespec ts;

  on_heap = (float*)calloc( N_DEFAULT, sizeof(float) );

  for ( int r = 0; r < ITER; r++ )
    {
      double t0 = CPU_TIME;
      
      for ( int i = 0; i < N_DEFAULT; i++ )
	on_heap[ i ] = (float)i;
      float volatile access = 0;
      for ( int i = 0; i < N_DEFAULT; i++ )
	access += on_heap[ i ] + 1.0;

      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }

  free ( on_heap );
  
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);
  
  printf("timing for accessing the heap with ptr arithmetic was: %g (sigma: %g)\n\n", time, std_dev);
  
  return 0;
}


int access_the_heap_dp ( void )
{
  
  double time    = 0;
  double std_dev = 0;
  struct  timespec ts;

  on_heap = (float*)calloc( N_DEFAULT, sizeof(float) );
  
  for ( int r = 0; r < ITER; r++ )
    {
      float register * entry = on_heap;
      const float register * stop  = on_heap + N_DEFAULT;

      double t0 = CPU_TIME;

      for ( ; ++entry < stop;)
	*entry = (float)(entry - on_heap);
      float volatile access = 0;
      for ( entry = on_heap; ++entry < stop;  )
	access += *entry + 1.0;
      
      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }

  free ( on_heap );
      
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);

  printf("timing for accessing the hep with reduced ptr arithmetic was: %g (sigma: %g)\n\n", time, std_dev);

  return 0;
}




int main ( int argc, char **argv )
{
  int   run = 0;
  
  if ( argc > 1 )
    run = (int)atoi( *(argv+1) );

  printf ( "\nrunning test.. " ); fflush(stdout);

  switch ( run )
    {
    case 0: access_the_stack ( ); break;
    case 1: access_the_stack_dp ( ); break;
    case 2: access_the_heap ( ); break;
    case 3: access_the_heap_dp ( ); break;
    default: printf("runs type %d is not available, try values in the range 0-3\n", run);
    }
  
  return 0;
}
