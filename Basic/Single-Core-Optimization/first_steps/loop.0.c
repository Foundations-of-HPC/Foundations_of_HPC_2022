/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
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
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>


/*
 * The purpose of this code is to show you some incremental
 * optimization in loops.
 * Conceptually, it is a loop over a number Np of particles
 * with tha aim of calculating the distance of each particle
 * from the center of the cells of a grid within a maximum distance
 * Rmax.
 *
 * The grid is actually not allocated.
 */


// look at the lecture about timing for motivations about the following macro
#define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)


#define ITER 30

int main(int argc, char **argv)
{
  
  double           *x, *y, *z, dist;
  double           half_size, Rmax;
  int              Np, Ng, i, j, k, p, r;
  int              nIter;
  
  // timing-related variables
  struct timespec ts;

  // get the number of particles to use
  Np = atoi( *(argv + 1) );
  // get the number of grid points
  Ng = atoi( *(argv + 2) );

  if ( argc > 3 )
    nIter = atoi( *(argv + 3) );
  else
    nIter = ITER;


  // allocate contiguous memory for particles coordinates
  x    = (double*)calloc(Np * 4, sizeof(double));
  y    = x + Np;
  z    = y + Np;

  // this quantity will be used to calculate the grid
  // center starting from the (corner) grid point
  half_size = 0.5 / Ng;

  // the maximum distance
  Rmax = 0.2;
  
  // initialize random number generator
  //srand48(clock());   // change seed at each call
  srand48(997766);    // same seed to reproduce results

  // initialize mock coordinates, in the interval [0, 1[
  printf("initialize coordinates..\n");
  for(i = 0; i < 3*Np; i++)
    x[i] = drand48();



  
  // ---------------------------------------------------
  // LOOP 0: the worst case
  // ---------------------------------------------------

  
  printf("LOOP 0 :: "); fflush(stdout);

  double ctime = 0;
  double dummy = 0;

  for ( r = 0; r < nIter; r++ )
    {

      double tstart = TCPU_TIME;
      
      for ( p = 0; p < Np; p++ )
	for ( i = 0; i < Ng; i++ )
	  for ( j = 0; j < Ng; j++ )
	    for ( k = 0; k < Ng; k++ )
	      {
		dist = sqrt( pow(x[p] - (double)i/Ng + half_size, 2) +
			     pow(y[p] - (double)j/Ng + half_size, 2) +
			     pow(z[p]  - (double)k/Ng + half_size, 2) );
		if(dist < Rmax)
		  dummy += dist;
	      }
      
      ctime += TCPU_TIME - tstart;
    }
  
  printf("\t%g sec [%g]\n", ctime / nIter, dummy / nIter);

  free(x);
  
  return 0;
}
