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
#include <sys/resource.h>
#include <sys/times.h>
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
 */


#define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)

#define ALIGN 16

int main(int argc, char **argv)
{
  
  double           *parts;
  double           Rmax;
  int              Np, Ng;
  
  
  // timing-related variables
  double          tstart, ctime;
  struct timespec ts;

  Np = atoi( *(argv + 1) );
  Ng = atoi( *(argv + 2) );
  

  // allocate contiguous memory for particles coordinates
  if ( posix_memalign((void**)&parts, ALIGN, Np * 3 * sizeof(double)) != 0 )
    {
      printf("some problem arose allocating %lu bytes of %d-bits aligned memory.\n", Np * 3 * sizeof(double), ALIGN);
      return 1;
    }

  Rmax = 0.2;
  
  // initialize random number generator
  //srand48(clock());   // change seed at each call
  srand48(997766);    // same seed to reproduce results

  // initialize mock coordinates
  printf("initialize coordinates..\n");
  for( int i = 0; i < 3*Np; i++)
    parts[i] = drand48();


  
  // ---------------------------------------------------
  // STEP 8b: let the compiler vectorize, second attempt
  // ---------------------------------------------------
  
  printf("LOOP 7 :: "); fflush(stdout);
  
  double dummy[4] = {0};
  double Rmax2 = Rmax * Rmax;
  double half_size = 0.5 / Ng;;
  double Ng_inv = (double)1.0 / Ng;
  double *jks;
  posix_memalign((void**)&jks, 16, Ng * sizeof(double));

  for( int i = 0; i < Ng; i++)
    jks[i] = (double)i * Ng_inv + half_size;
    
  int Np3 = Np * 3;
    ctime = 0;

    // now let's imagine that we can store the particles' coordinates keeping x, y and z
    // contiguous by bunch, i.e. :
    // parts = { x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4, ...}
    
  tstart = TCPU_TIME;

  for( int p = 0; p < Np3; p += 12 )

    for( int i = 0; i < Ng; i++)
      {
	double dx2[4] = {(parts[p] - jks[i])*(parts[p] - jks[i]),
			 (parts[p+1] - jks[i])*(parts[p+1] - jks[i]),
			 (parts[p+2] - jks[i])*(parts[p+2] - jks[i]),
			 (parts[p+3] - jks[i])*(parts[p+3] - jks[i]) };
       #pragma ivdep    
	for( int j = 0; j < Ng; j++)
	  {
	    double dy2[4] = {(parts[p+4] - jks[j])*(parts[p+4] - jks[j]),
			     (parts[p+5] - jks[j])*(parts[p+5] - jks[j]),
			     (parts[p+6] - jks[j])*(parts[p+6] - jks[j]),
			     (parts[p+7] - jks[j])*(parts[p+7] - jks[j]) };
	    
	    double dist2_xy[4] = {dx2[0] + dy2[0],
				  dx2[1] + dy2[1],
				  dx2[2] + dy2[2],
				  dx2[3] + dy2[3] };
	    
	   #pragma ivdep    	    
	    for( int k = 0; k < Ng; k++)
	      {
		double dz2[4] = {(parts[p+8] - jks[k])*(parts[p+8] - jks[k]),
				 (parts[p+9] - jks[k])*(parts[p+9] - jks[k]),
				 (parts[p+10] - jks[k])*(parts[p+10] - jks[k]),
				 (parts[p+11] - jks[k])*(parts[p+11] - jks[k]) };
		
		double dist2[4] = {dist2_xy[0] + dz2[0],
				   dist2_xy[1] + dz2[1],
				   dist2_xy[2] + dz2[2],
				   dist2_xy[3] + dz2[3]};

		
		dummy[0] = (dist2[0] < Rmax2)*sqrt(dist2[0]);
		dummy[1] = (dist2[1] < Rmax2)*sqrt(dist2[1]);
		dummy[2] = (dist2[2] < Rmax2)*sqrt(dist2[2]);
		dummy[3] = (dist2[3] < Rmax2)*sqrt(dist2[3]);
	      }
	  }
      }
	    
  ctime += TCPU_TIME - tstart;
  
  printf("\t%g sec [%g]\n", ctime, dummy);

  free(jks);
  free(parts);
  
  return 0;
}
