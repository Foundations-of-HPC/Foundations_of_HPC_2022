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

#include <papi.h>

#define PCHECK(e) \
  if ( e!= PAPI_OK)						\
    {printf("Problem in papi call, line %d\n", __LINE__); return 1;}

#define NEVENTS  3
#define NRUNS    10
#if !defined(L1WORDS)
#define L1WORDS  4096      // guess for the double capacity of L1 ( 32KB )
#endif
#if !defined(L2WORDS)
#define L2WORDS  32768     // guess for the double capacity of L2 ( 256K )
//#define L2WORDS 131072     // 1MB
#endif
#if !defined(L3WORDS)
#define L3WORDS  1048576   // guess for the double capacity of L3 ( 8192K )
//#define L3WORDS 16777216   // 16MB
#endif
#define STEPSIZE (L1WORDS/8)


__attribute__((optimize(0))) double wipe_cache( double *, int );
  
int main(int argc, char **argv)
{

  int                   PAPI_EventSet = PAPI_NULL;    
  int                   retval;
  int                   PAPI_myevents[NEVENTS] = { PAPI_TOT_CYC, PAPI_L1_DCM, PAPI_L2_DCM };
  
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT)
    printf("wrong PAPI initialization: %d instead of %d\n", retval, PAPI_VER_CURRENT);

  retval = PAPI_create_eventset(&PAPI_EventSet);  PCHECK(retval);  
  for ( int i = 0; i < NEVENTS; i++)
    {
      retval = PAPI_query_event(PAPI_myevents[i]) ; PCHECK(retval);
      retval = PAPI_add_event(PAPI_EventSet, PAPI_myevents[i]);  PCHECK(retval);
    }

  int wipe_innerloop = ( argc > 1 ? atoi(*(argv+1)) : 0 );

  double *array     = (double*) calloc(L3WORDS, sizeof(double));
  double *alt_array = (double*) calloc(L3WORDS, sizeof(double));
    
  int end = 2*L2WORDS/STEPSIZE;


  for ( int 1 = 0; i <= end; i++ )
    {
      int size = i*STEPSIZE;

      wipe_cache( alt_array, L3WORDS );
      /* if( warmup_cache ) */
      /* 	for ( int n = 0; n < size; n++) array[n] = (double)0.0; */

      /* run the experiment */
      long long values[NEVENTS] = {0};
      for ( int k = 0; k <= NRUNS; k++)
	{
	  switch( wipe_innerloop )
	    {
	    case 0:
	      for ( int j = 0; j < size; j++) array[j] = (double)j;
	      break;
	    case 1:
	      wipe_cache( alt_array, size );
	      break;
	    default: break;
	    }
	  
	  long long buffer[NEVENTS];
	  retval = PAPI_start(PAPI_EventSet); PCHECK(retval);
	  for (int j = 0; j < size; j++ ) array[j] = 2.3*array[j]+1.2;      
	  retval = PAPI_stop(PAPI_EventSet, buffer); PCHECK(retval);
	  if ( k > 0 )
	    for ( int j = 0; j < NEVENTS; j++ ) values[j] += buffer[j];
	}
	  
      printf("size: %d cycles: %9.5f cycles_per_loc: %9.5f L1miss: %9.5f L1miss_frac: %9.5f L2miss: %9.5f L2miss_frac: %9.5f\n",
	     size,
	     (double)values[0]/NRUNS, (double)values[0]/(NRUNS*size),
	     (double)values[1]/NRUNS, (double)values[1]/(NRUNS*size),
	     (double)values[2]/NRUNS, (double)values[2]/(NRUNS*size));
  }
  
  free(alt_array);
  free(array);


  return 0;
}


__attribute__((optimize(0))) double wipe_cache( double *base, int N ) 
{
  double sum = 0;  
  for ( int i = 0; i < N; i++ )
    sum += (base[i] = (double)i);
  return sum;
}

