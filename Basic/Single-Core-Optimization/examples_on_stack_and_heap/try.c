
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


/* ----------------------------------------------------------------- *
 *                                                                   *
 *                                                                   *
 *                                                                   *
 * ----------------------------------------------------------------- */


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

int main ( int argc, char **argv )
{


  printf("\targc resides at %p and its value is %d\n",
	 &argc, argc );
	 
  for ( int ii = 0; ii < argc; ii++ )
    {
      printf("pointer for argument %d is %p, argument %d is %s\n",
	     ii, argv+ii, ii, *(argv+ii) );

    }

 #define N 4
  
  int i;
  int *iarray;

  double d;
  double *darray;

  iarray = (int*)malloc( N*sizeof(int) );
  darray = (double*)malloc ( N*sizeof(double) );

  printf("\n\n");
  
  for ( int j = 0; j < N; j++ )
    printf ( "element %d of int array lives at %p %p"
	     " - it lives at %ld positions from the beginning of the int array\n\n",
	     j, iarray+j, &iarray[j],
	     &iarray[j] - &iarray[0]);
  

  free(darray);
  free(iarray);
  
  return 0;
}

