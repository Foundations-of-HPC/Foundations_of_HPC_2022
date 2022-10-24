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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "mypapi.h"
#define CYC 0
#define INS 1
#define BRN 2
#define MBR 3
#define ST_CYC 4


#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)


#define NSHOTS 100000000
#define ITER   10

int main ( int argc, char **argv )
{
  int    N = NSHOTS;
  double time_avg = 0;
  double time_std_dev = 0;
  struct timespec ts;

  PAPI_INIT;

  N    = ( argc > 1 ? atoi( *(argv+1) ) : NSHOTS);
 
  printf("%d iterations of %u shots\n", ITER, NSHOTS);
  srand48(time(NULL));

  unsigned long long shot_in = 0;
  for ( int R = 0; R < ITER; R++ )
    {
      PAPI_FLUSH_BUFFER;
      double tstart = CPU_TIME;      
      PAPI_START_CNTR;

      for ( int shot = 0; shot < N; shot++ )
	{
	  double x  = drand48();                      /* draw 2 pseudo-rnd in [0,1]       */
	  double y  = drand48();
	  
	  shot_in += ( (x*x + y*y) < 1.0 );
	}

      double elapsed = CPU_TIME - tstart;
      PAPI_STOP_CNTR;      

      time_avg     += elapsed;
      time_std_dev += elapsed*elapsed;
      
    }

  
  printf("PI is %g\n", ((double)shot_in)/ITER/N*4.0 );

  time_avg     /= ITER;
  time_std_dev  = sqrt(time_std_dev/(ITER-1) - time_avg*time_avg);
  printf("\nseconds          : %7.4g +- %6.4g\n", time_avg, time_std_dev);


  

#ifdef USE_PAPI
							    /* process the counters' infos         */
  long double papi_avg[PAPI_EVENTS_NUM];
  for ( int cc = 0; cc < PAPI_EVENTS_NUM; cc++ )
    {
      papi_avg[cc] = ((long double)PAPI_GET_CNTR(cc)/ N) / ITER;
      papi_stddev_values[cc] = sqrt(((papi_stddev_values[cc] / N) / N) / ITER  - papi_avg[cc]*papi_avg[cc]);
    }


							    /* output useful numbers               */
  
  printf("CPE              : %6s%7.4Lg +- %5.3Lg\n"         /* cycles per element (of the array)   */
	 "IPE              : %13.9Lg +- %7.4Lg\n"	    /* instructions per element            */
	 "BRANCHES_PE      : %13.9Lg +- %7.4Lg\n"	    /* conditional branches per element    */
	 "MISP_BRANCHES_PE : %13.9Lg +- %7.4Lg "	    /* mispredicted cond. brn. per element */
	 " ( tot: %13.9Lg )\n" 				    /* total mispredicted cond. branches   */
	 "STALLED CYC      : %13.9Lg +- %7.4Lg\n",
	 "", papi_avg[CYC], papi_stddev_values[CYC],
	 papi_avg[INS], papi_stddev_values[INS],
	 papi_avg[BRN], papi_stddev_values[BRN],
	 papi_avg[MBR], papi_stddev_values[MBR],
	 papi_avg[MBR]*N,
	 papi_avg[ST_CYC], papi_stddev_values[ST_CYC]);
#endif

  return 0;
}
