
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
 * a very basic example of stack smashing                            *
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


#define SIZE 3

int main( void ) 
{
 #if defined(UP_FENCE)
  int myprotection[ SIZE ];
 #endif
  int array[ SIZE ];
 #if defined(DOWN_FENCE)
  int myprotection[ SIZE ];
 #endif

  register long long int myRBP __asm__("rbp");
  register long long int myRSP __asm__("rsp");


  //
  // the following print will let you
  // understand where the local variables
  // are in the stack
  // "distances" are expressed relatively to
  // BP, which is the largest address in
  // the current stack frame, so they are
  // negative.
  //
  printf("RSP     : %p\n"          // the top of the stak (remind: it is the lowest address)
	 "RBP     : %p\n\n"        // the base of the stack
	 "RBP-RSP : %ld\n"         // the size of the stack
	 "array[0]: %ld\n",	   // the position of the first element of array
	 (void*)myRSP, (void*)myRBP,
	 (void*)myRBP - (void*)myRSP,
	 (void*)&array[0] - (void*)myRBP);
  
 #if defined(UP_FENCE) || defined(DOWN_FENCE)
  printf("fence[0]: %ld\n", (void*)&myprotection[0] - (void*)myRBP );
  if( ((void*)&myprotection[0] - (void*)&array[0]) < 0 )
    printf("\n\n"
	   ">>>  fence is down-hill, it will not work:\n"
	   "     as you may have noticed, array grows up-hill\n"
	   "     meaning that larger indexes have larger addresses.\n"
	   "     Try to compile using an up-hill fence:\n"
	   "        gcc -DUP_FENCE -o stacksmash_simple stacksmash_simple.c\n\n");

  // the relative position of the local variables is compiler-dependent;
  // there are no standards, the only criterion is usually to keep all the
  // variables aligned to their size
  // 
 #endif

    
  for ( int ii = 0; ii <= SIZE+2; ii++ )
    //
    // the error in this for cycle is the
    // termination condition, which should be
    //    ii < SIZE
    // if there are no fences and the code
    // is compiled without stack protector
    // (like the one implemented in gcc),
    // array[] is close to the base of stack
    // and this for cycle will corrupt the
    // return address causing a seg fault
    // with high probability
    //
    {
      if ( ii == 0 )
	printf("ii      : %ld\n", (void*)&ii - (void*)myRBP);   // the position of the for cycle counter
      
      printf("accessing %d-th element: %ld\n", ii, (void*)&array[ii] - (void*)myRBP);
      array[ ii ] = ii;
    }


  //
  // EXERCISE: this bug is trivial, try to smash the SP, for instance with a different for cycle
  // for ( int ii = SIZE; --ii > -3; )
  // 
  
  return 0;
}
