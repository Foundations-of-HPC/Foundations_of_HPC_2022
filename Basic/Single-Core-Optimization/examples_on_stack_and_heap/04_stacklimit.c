
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
 * a simple example of stack smashing                                *
 * 3 different functions, func1, func2 and func3, try to allocate    *
 * some memory:                                                      *
 *   - func1 does it normally by heap allocation                     *
 *   - func2 does it by declaring a local array of the needed size   *
 *     this one will cause the seg fault violation, because the      *
 *     array's size is purposely as large as the default sys limit   *
 *     of the stack                                                  *
 *   - func3 does it by dynamic allocating on the stack              *
 *     if the code reached this point, it would cause a violation    *
 *     because there would not be enough memory available            *
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

#define _GNU_SOURCE                // this is to avoid problems with the 
                                   //   definition of alloca()
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if _XOPEN_SOURCE >= 600
#  include <strings.h>
#endif
#include <sys/resource.h>


void func1(unsigned int S)
/*
 * array here is dynamically allocated, and as such
 * memory is allocated into the heap, not into the
 * stack.
 * that's why it does not lead to a seg fault when
 * called even if S*sizeof(int) is larger than the
 * stack size
 */
{
  char *array;

  array = (char*)malloc(S * sizeof(char));

  for ( int i = 0; i < S; i++ )
	array[i] = i;

  free(array);
  
  printf("dynamic allocation of %zd bytes on the heap has been done\n", S * sizeof(char)); fflush(stdout);
  return;	
}


void func2(unsigned int S)
/*
 * usually the linux stack limit is set at 8192KB;
 * if this is still the case at the moment of executing
 * this routine, it will cause a seg fault when called.
 * In fact, it will allocate a local array of 8192KB
 *
 * try to set a different stack limit through
 *   ulimit -s <limit>
 * or to decrease the dimension of array 
 * (decreasing STACKSMASH) and magically the 
 * segfault will disappear
 */
{
  char array[S];          // try to get S KB on the stack
			  // at this point you will not get any seg fault, yet:
			  // the top pf the stack will be set up to the maximum
			  // that, however, leave less space than needed because
			  // there are already some variables on the stack

  for ( int i = 0; i < S; i++ )
    // you will start to access the array, simply by offsetting the
    // start of the array on the stack.
    // seg fault will come when you hit the top of the stack
	array[i] = i;
  
  printf("static  allocation of %zd bytes on the stack has been done\n", S * sizeof(char)); fflush(stdout);
  return;
}


void func3(unsigned int S)
/*
 * This routine will try to dynamically allocate S
 * integer on the stack.
 * If you succeeded in enlarging the stack, if needed,
 * there will be no segfault.
 * Otherwise...
 */
{
  char *array = (char*) alloca ( S* sizeof(char) );
  for ( int i = 0; i <  S; i++ )
    array[i] = i;

  // you don't need to free from stack
  
  printf("dynamic allocation of %zd bytes on stack the has been done\n", S * sizeof(char)); fflush(stdout);
  return;
}




int main(int argc, char **argv)
{
  unsigned int  S;
  struct rlimit stack_limits;
  
  getrlimit(RLIMIT_STACK, &stack_limits);
      

  // get the memory to be put in the stack (in KB)
  if(argc > 1)
    
    S = (unsigned int)((int)(atof(*(argv+1)) * 1024));
  
  else
    {
      S = stack_limits.rlim_cur;
      
      printf("\nyou did not give a memory amount to be allocated, "
	     "I'll use the current soft stack limit of %u\n(hard limit is set to %d)\n",
	     S, (int)stack_limits.rlim_max);
    }
  printf("\n");
  
  // call func1, it dynamically allocates in the heap
  func1(S);

  
  // call func2, it statically allocates in the stack
  func2(S);           //  <<----  THAT WILL FAIL if S is too large

  
  // call func3, it dynamically allocates in the stack
  func3(S);           //  <<----  THAT WILL FAIL if STACKSMASH is too large
                      //          but probably you won't notice it because of
                      //          the segfault raised in the previous function



  return 0;
}
