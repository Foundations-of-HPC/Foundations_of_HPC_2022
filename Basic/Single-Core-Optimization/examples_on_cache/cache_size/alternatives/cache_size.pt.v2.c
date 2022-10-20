/* =============================================================================
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

#include "ptiming.h"

#define CPU_TIME ({ struct timespec ts; double t; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ); \
      t = (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9; t;})

//#define CPU_TIME ( clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9 )


#define NRUNS    10
#define L1WORDS  4096      // guess for the double capacity of L1 ( 32KB )
#define L2WORDS  32768     // guess for the double capacity of L2 ( 256K )
#define L3WORDS  1048576   // guess for the double capacity of L3 ( 8192K )
#define STEPSIZE (L1WORDS/8)



__attribute__((optimize(0))) double wipe_cache( double *, int );

  
int main(int argc, char **argv)
{
  int warmup_cache = ( argc > 1 ? atoi(*(argv+1)) : 0 );
  int wipe_or_warmup_innerloop = ( argc > 2 ? atoi(*(argv+2)) : 0 );

  init_cycles_counter();
  double *cache_wide_array = (double*) calloc(2*L3WORDS, sizeof(double));
  double *alt_cache_wide_array = (double*) calloc(L3WORDS, sizeof(double));
  
  double *offsets[2];  
  offsets[0] = cache_wide_array;
  offsets[1] = cache_wide_array + L3WORDS;
  
  int end = 2*L2WORDS/STEPSIZE;
  
  for ( int i = 1; i <= end; i++ )
    {
      int size = i*STEPSIZE;
      double *array = offsets[i%2];
      
      if( warmup_cache )
	for ( int n = 0; n < L1WORDS; n++) array[n] = (double)0.0;

      uint64_t cycles = 0;
      double   timing = 0;
      for( int r = 0; r < NRUNS; r++ ) {

	switch( wipe_or_warmup_innerloop )
	  {
	  case 0:
	    for ( int n = 0; n < size; n++) array[n] = (double)n;
	    break;
	  case 1:
	    wipe_cache( alt_cache_wide_array, L3WORDS );
	    break;
	  default: break;
	  }

	//struct timespec ts;
	double tstart = CPU_TIME;
	uint64_t clk1 = read_cyc_counter();
	for (int j = 0; j < size; j++ ) (array[j] = 2.3*array[j]+1.2);
	uint64_t clk2 = read_cyc_counter();
	double telapsed = CPU_TIME - tstart;
	
	cycles += (clk2 - clk1) - clock_info.clock_overhead_direct*2;
	timing += telapsed;
      }
      
      printf("size: %d tot_cycles %9.5f cycles_per_loc: %9.5f sec_per_loc: %g\n",
	     size, (double)cycles/NRUNS, (double)cycles/(NRUNS*size), timing / (NRUNS*size) );
    }

  free(alt_cache_wide_array);
  free(cache_wide_array);

  return 0;
}


__attribute__((optimize(0))) double wipe_cache( double *base, int N ) 
{
  double sum = 0;  
  for ( int i = 0; i < N; i++ )
    sum += (base[i] = (double)i);
  return sum;
}
