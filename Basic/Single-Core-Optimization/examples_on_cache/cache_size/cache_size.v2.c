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


/* :--------------------------------------------------------------- :
   :                                                                :
   :  include relevant headers                                      :
   :                                                                :
   :--------------------------------------------------------------- : */

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
#include <time.h>     // we need this for clock_gettime()
#include <float.h>    // we need this for having DBL_MAX

#include "mypapi.h"

/* :--------------------------------------------------------------- :
   :                                                                :
   :  useful defines and type definitions                           :
   :                                                                :
   :--------------------------------------------------------------- : */

#define CPU_TIME ({ struct timespec ts; double t; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ); \
      t = (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9; t;})


typedef unsigned long long int ull;

#define NITER    10
#define MAX_STRIDE 64
#define WORD     sizeof(ull)

#include "mycache.h"
#define STEPSIZE (L1WORDS/8)


/* :--------------------------------------------------------------- :
   :                                                                :
   :  function prototypes                                           :
   :                                                                :
   :--------------------------------------------------------------- : */

void insert( double *, double, int );
__attribute__((optimize(0))) double estimate_timing_overhead( );
__attribute__((optimize(0))) double estimate_loop0_toverhead(int, int, int, double );
__attribute__((optimize(0))) void   estimate_loop0_poverhead(int, int, int, ull *);


/* :--------------------------------------------------------------- :
   :                                                                :
   :  here it comes the main                                        :
   :                                                                :
   :--------------------------------------------------------------- : */


int main(int argc, char **argv)
{
  int verbose  = (argc > 1 ? atoi(*(argv+1)) : 0 );
  int maxsize  = 2*L3WORDS;
  ull *array   = (ull*) calloc(maxsize, sizeof(ull));
  double overhead_t;
  
  
  PAPI_INIT;

  int n_sizesteps = 0;
  for ( ; (STEPSIZE << n_sizesteps) <= maxsize; n_sizesteps++ );
  int n_strides = 0;
  for( ; (1 << n_strides) <= MAX_STRIDE; n_strides++ );

  overhead_t = estimate_timing_overhead();
  if( verbose )
    {
      printf("timing function overhead is : %g nsec\n",
	     overhead_t * 1e9 );      
      printf("\n");
      
      printf("using %d steps for data size, from %lu to %d\n"
	     "using %d steps for stride, from %d to %d\n",
	     n_sizesteps, STEPSIZE, maxsize,
	     n_strides, 1, MAX_STRIDE );
      
      printf("-----------------------------------------------------\n\n");
    }
  
  
  double      *timings = (double*)calloc( n_sizesteps * n_strides, sizeof(double) );
  long double *results = (long double*)calloc( n_sizesteps * n_strides * PAPI_EVENTS_NUM, sizeof(long double) );

  FILE *file;
  file = fopen("cache_size.v2.dat", "w");
  
  for ( int i = n_sizesteps-1; i >= 0; i-- )
    {
      // determines the data size for this step
      //
      int size = STEPSIZE << i;

      // determines the maximum stride for this size
      //
      int this_n_strides = n_strides;
      while ( (this_n_strides >= 0) && ((size / (1 << this_n_strides)) <= 32) )
	this_n_strides--;

      // -------------------------------
      // print infos on the current step
      //
      printf("loops for size %d*%lu, max stride %d, %4.3g in L1 capacity units, "
	     "%4.3g in L3 capacity units (step %d / %d )\n",
	     size, sizeof(ull), 1 << (this_n_strides-1),
	     (double)size*WORD/L1SIZE, (double)size*WORD/L3SIZE, i+1, n_sizesteps );

      fprintf(file, "\"size ");
      double Ldsizes[3] = {(double)size/L1WORDS, (double)size/L2WORDS, (double)size/L3WORDS};
      for( int l = 0; l < 3; l++ ) {
	if( Ldsizes[l] < 0.125 )
	  fprintf( file, "<0.125L%d ", l+1);
	else
	  fprintf( file, "%4.1fL%d ", Ldsizes[l], l+1); }
      fprintf(file, "\"\n");
      // -------------------------------


      // iterates from stride 1 to the maximum stride
      //
      for ( int stride_count = 0; stride_count < this_n_strides; stride_count++ )
	{

	  // +++++++++++++++++++++++++++
	  // set-up local variables
	  //
	 #define N_TIMINGS 10
	  double      elapsed[N_TIMINGS];                            // this will store the best 10 timings for the inner loop
	  double      tot_elapsed        = 0;	                     // this will store the total time spent in iterating the inner loop
	  int         repetitions        = 0;	                     // how many times the inner loop is performed
	  int         stride             = 1 << stride_count;        // the current stride
	 #if defined(USE_PAPI)    	  
	  long double values[PAPI_EVENTS_NUM] = {0.0};               // the accumulators for PAPI events
	 #endif

	  for( int kk = 0; kk < N_TIMINGS; kk++ )
	    elapsed[kk] = DBL_MAX;                                   // this allows us to simplify the routine insert() 
	  
	  // +++++++++++++++++++++++++++
	  // print infos on this stride-step
	  //
	  printf("\tloops with stride %d.. ", stride ); fflush(stdout);

	  // +++++++++++++++++++++++++++
	  // estimate the loop timing overhed
	  //	  
	  double overhead_loop0_t = estimate_loop0_toverhead( NITER, size, stride, overhead_t);
	  if ( verbose )
	    printf("loop timing overhead is %5.4g sec.. ", overhead_loop0_t );
	  
	 #if defined(USE_PAPI)
	  // +++++++++++++++++++++++++++
	  // estimate the loop events overhead
	  //
	  ull overhead_loop0_p[PAPI_EVENTS_NUM];
	  estimate_loop0_poverhead( NITER, size, stride, overhead_loop0_p);
	  if( verbose ) {
	    printf("loop event ovrh is " );
	    for( int i = 0; i < PAPI_EVENTS_NUM; i++ )
	      printf("%llu ", overhead_loop0_p[i]);
	    printf(" | "); fflush(stdout);}
	 #endif

	  // +++++++++++++++++++++++++++
	  //   REPETEAD INNER LOOP
	  // +++++++++++++++++++++++++++
	  // repeat the inner loop until either at least 200 repetitions
	  // have been performed
	  // or at least one second has elapsed
	  //
	  while( (repetitions < 200 ) ||
		 ((tot_elapsed < 1.0) && (repetitions >= 200)) )
	    {
	      // ...................
	      // warm-up the cache
	      //
	      int drag_size = (size > L3WORDS ? L3WORDS : size);
	      for ( int k = 0; k < drag_size; k++ )
		array[k] = k;

	      // ...................
	      // prepare the variables
	      // used for the unrolling
	      //
	      int off2 = stride + stride;
	      int off3 = off2 + stride;
	      int off4 = off2 + off2;

	      // ...................
	      // take the initial time
	      //
	      double tstart = CPU_TIME;

	      // .............................
	      // .............................
	      // the inner loop: for NITER times
	      // it traverses the array with
	      // a given stride
	      //
	      PAPI_FLUSH;                                      // emtpy the counters
	      PAPI_RESET;
	      PAPI_START_CNTR;                                 // start the counters
	      
	      for ( int j = 0; j < NITER; j++ )
		{
		  ull acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
		  for ( int k = 0; k < size; k += off4 ) {
		    acc0 += array[k];
		    acc1 += array[k+stride];
		    acc2 += array[k+off2];
		    acc3 += array[k+off3]; }
		}

	      PAPI_STOP_CNTR;                                  // stop the counters
	      // ............................
	      // ............................
	      
	     #if defined(USE_PAPI)
	      // ...................
	      // accumulate the counters
	      //

	      for ( int k = 0; k < PAPI_EVENTS_NUM; k++ )
		// if the kth counter is larger than the
		// estimated overhead for the loop accounts for it
		//
		if ( PAPI_GET_CNTR(k) > overhead_loop0_p[k] ) {		  
		  long double v = ((long double)PAPI_GET_CNTR(k)) -   // first subtract the overhead
		    overhead_loop0_p[k]; 		  
		  v /= (NITER*size/stride);                           // get the counter value per element accessed
		  
		  if (  (k == 3) && v > 10 )                          // k = 3 is the L2_DCM event; for some reasons
		    v = 0;                                            // it seems to be unstable when the counts are
								      // very few; as such it may deliver unreasonable
								      // numbers that we try to exclude with this
								      // by-hand condition
		  
		  values[k] += v;}                                    // accumulate
	     #endif

	      // ...................
	      // take the time after the inner loop
	      // and accumulate in the total time
	      // spent in the inner loop
	      //
	      double this_time = CPU_TIME - tstart;
	      tot_elapsed += this_time;
	      

	      // correct for the overheads
	      //
	      this_time = (this_time  - (overhead_t + overhead_loop0_t))/NITER;
	      if ( this_time > 0 )
		// if we are not at the resolution limit,
		// insert the current timing in the 10-best
		// array with an insertion sort
		//
		insert( elapsed, this_time, N_TIMINGS );
	      
	      repetitions++;
	    }


	  // +++++++++++++++++++++++++++
	  //  END of the REPETITIONS of THE INNER LOOP
	  // +++++++++++++++++++++++++++
	  // here we store the informations about
	  // performances in this size, stride pair
	  //

	  // +++++++++++++++++++++++++++
	  // average the 10 best timings
	  // to estimate the inner loop timing
	  //
	  double time_elapsed = 0;
	  int    how_many_timings = 0;
	  for( int k = 0; k < N_TIMINGS; k++ ) {
	    how_many_timings += (elapsed[k] < DBL_MAX);
	    time_elapsed += (elapsed[k] < DBL_MAX) * elapsed[k]; }

	  // +++++++++++++++++++++++++++
	  // get the timing per processed element
	  //
	  double n_el = (double)(size/stride);
	  double t_per_el = time_elapsed / how_many_timings / n_el;
	  printf(" elapsed time is %g sec (%d repetitions, ~%g sec/element)\n",
		 tot_elapsed, repetitions, t_per_el );
	  //
	  // store the timing
	  //
	  timings[ i*n_strides + stride_count ] = t_per_el;
	  
	 #if defined(USE_PAPI)
	  // +++++++++++++++++++++++++++
	  // get the counters values
	  // and store them
	  //
	  for ( int k = 0; k < PAPI_EVENTS_NUM; k++ )
	    results[ (i*n_strides + stride_count)*PAPI_EVENTS_NUM + k ] = values[k]/repetitions;
	  
	 #endif


	  // +++++++++++++++++++++++++++
	  // write relevant informations
	  // on the output file
	  //
	  fprintf(file, "%d %.3g ", 1<<stride_count, timings[i*n_strides + stride_count]);
	 #if defined(USE_PAPI)
	  for( int k = 0; k < PAPI_EVENTS_NUM; k++ )
	    fprintf(file, "%.4Lg ", results[(i*n_strides + stride_count)*PAPI_EVENTS_NUM+k]);
	 #endif
	  fprintf(file, "\n");

	  
	}      // ==== closes the loop over the strides ====

      
      // ------------------
      // separate the data set
      // for this size in the
      // output file
      //
      fprintf(file, "\n\n"); fflush( file );
      
    }      // ==== closes the loop over the sizes ====


  
  fclose(file);
  
  
  free( results );
  free( timings );
  free( array );
  
  
  return 0;
}




/* :--------------------------------------------------------------- :
   :                                                                :
   :  functions called from the main                                :
   :                                                                :
   :--------------------------------------------------------------- : */


void insert( double *array, double t, int N )
/*
 * .................................................
 * a simple insertion sort when the target array is
 * initialized to large numbers, where "large" means
 *  "large enough to be sure that every possible item
 *   t will be smaller".
 * In this case we initialize the array to the max
 * possible double-precision value.
 * .................................................
 *
 * this routine inserts the item "t" into "array"
 * so that the array is always sorted.
 * N is the length of the array
 *
 */
{
  if( array[N-1] > t )
    {
      array[N-1] = t;
      int last = N-2;
      while( (last >= 0) && (array[last] > array[last+1]) )
	{
	  double _t_    = array[last];
	  array[last]   = array[last+1];
	  array[last+1] = _t_;
	  last--;
	}
    }

  return;
}


void _insert_( double *array, double t, int N, int r)
/*
 * a simple insertion sort when the target array is
 * initialized to zero instead of to large numbers.
 *
 * this routine inserts the item "t" into "array"
 * so that the array is always sorted.
 * N is the length of the array, while r is the
 * ordinal number of the item t that is to be inserted.
 * 
 * If r >= N we insert the item at the bottom of the
 * array, otherwise we insert it at position r.
 */
{
  int last = N-1;
  
  if( r < N ) {
    array[r] = t;
    last = r-1; }
  
  if( array[last] > t )
    {
      if( r < N )
	array[r] = array[last];
      array[last] = t;
      last--;
      while( (last >= 0) && (array[last] > array[last+1]) )
	{
	  double _t_      = array[last];
	  array[last]   = array[last+1];
	  array[last+1] = _t_;
	  last--;
	}
    }

  return;
}
  

__attribute__((optimize(0))) double estimate_timing_overhead( )
/*
 * estimate the overhead of the timing routine itself
 */
{
 #define MANY_ITER 125000
  double tstart = CPU_TIME;
  for( unsigned int j = 0; j < MANY_ITER; j++ )
    {
      double t0 = CPU_TIME;
      double t1 = CPU_TIME;
      double t2 = CPU_TIME;
      double t3 = CPU_TIME;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      t2 = CPU_TIME;
      t3 = CPU_TIME;
    }
  double overhead1 = (CPU_TIME - tstart)/(MANY_ITER*8+1);

  double overhead2 = 0;
  for( unsigned int j = 0; j < MANY_ITER; j++ )
    {
      double t0 = CPU_TIME;
      double t1 = CPU_TIME;
      overhead2 += t1-t0;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      overhead2 += t1-t0;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      overhead2 += t1-t0;
      t0 = CPU_TIME;
      t1 = CPU_TIME;
      overhead2 += t1-t0;
    }
  overhead2 /= (MANY_ITER*8+1);
  
 #undef MANY_ITER

  return (overhead1+overhead2)/2;
}



__attribute__((optimize(0))) double estimate_loop0_toverhead(int N, int size, int stride, double tovrhd )
/*
 * estimate the overhead of the inner loop in seconds
 */
{
  #define MANYLOOPS 100

 #if defined(USE_PAPI)    
  long double values[PAPI_EVENTS_NUM];
 #endif
  int off4 = stride*4;
  
  double tstart = CPU_TIME;
  for( int k = 0; k < MANYLOOPS; k++ )
    {
      PAPI_RESET;
      PAPI_START_CNTR;
      for ( int j = 0; j < N; j++ )
	{
	  ull acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
	  	  
	  for ( int k = 0; k < size; k+=off4 ) {
	    acc0++; acc1++; acc2++; acc3++;}
	}
      PAPI_STOP_CNTR;
     #if defined(USE_PAPI)    
      for ( int k = 0; k < PAPI_EVENTS_NUM; k++ ) values[k] += (long double)PAPI_GET_CNTR(k);
     #endif
      PAPI_FLUSH;
    }
  double overhead = (CPU_TIME - tstart)/MANYLOOPS - tovrhd;  
  #undef MANYLOOPS
  return overhead;
}


__attribute__((optimize(0))) void estimate_loop0_poverhead(int N, int size, int stride, ull *povrhd )
/*
 * estimate the overhead of the inner loop in terms of CPU events
 */
{
 #if defined(USE_PAPI)
 #define MANYLOOPS 100

  long double values[PAPI_EVENTS_NUM] = {0};
  int off4 = stride * 4;
  
  PAPI_FLUSH;
  PAPI_START_CNTR;
  for( int k = 0; k < MANYLOOPS; k++ )
    {
      for ( int j = 0; j < N; j++ )
	{
	  ull acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
	  for ( int k = 0; k < size; k+=off4 ) {
	    acc0++; acc1++; acc2++; acc3++;}
	}
      for ( int k = 0; k < PAPI_EVENTS_NUM; k++ ) values[k] += (long double)PAPI_GET_CNTR(k);	        
    }
  PAPI_STOP_CNTR;
  for ( int k = 0; k < PAPI_EVENTS_NUM; k++ ) povrhd[k] = PAPI_GET_CNTR(k)/MANYLOOPS;
  PAPI_FLUSH;
  
 #undef MANYLOOPS
 #else

 #endif
  return;
}

