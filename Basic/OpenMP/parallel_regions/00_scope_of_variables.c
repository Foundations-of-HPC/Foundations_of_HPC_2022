
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


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <omp.h>


int main( int argc, char **argv )
{
  int    i;

  /* here there is no parallelism
   */
  register unsigned long long base_of_stack asm("rbp");
  register unsigned long long top_of_stack asm("rsp");

/* Here we're getting some information on the process
 * SYS_gettid is the thread ID or process ID?
 */

  printf( "\nmain thread (pid: %ld, tid: %ld) data:\n"
	  "base of stack is: %p\n"
	  "top of stack is : %p\n"
	  "&i is           : %p\n"
	  "   rbp - &i     : %td\n"
	  "   &i - rsp     : %td\n"
	  "\n\n",
	  getpid(), syscall(SYS_gettid),
	  (void*)base_of_stack,
	  (void*)top_of_stack,
	  &i,
	  (void*)base_of_stack - (void*)&i,
	  (void*)&i - (void*)top_of_stack );
  
  // just test who is the private i for each thread
  // Here we're opening a parallel region.
  /* Golden rule: whatever is defined outside of the parallel region
   * is SHARED. But in this case we're saying that we want i to be private. The 
   * default would be i is shared.
   * There will be copies of the following parallel region for each thread
   * We will be able to see the different addresses of the different stacks
   * If a variable is shared, every thread will see the same address for it,
   * otherwise it is private.
   */
#pragma omp parallel private(i)
  {
    long int me = omp_get_thread_num(); //This function returns the ID of the thread that is called -> same
					//result as SYS_gettid
    unsigned long long my_stackbase;
    __asm__("mov %%rbp,%0" : "=mr" (my_stackbase)); //my_stackbase will be a variable in the stack. I
						    // want to see its address so that I can prove each thread
						    // has a different stack
    
    printf( "\tthread (tid: %ld) nr %ld:\n"
	    "\t\tmy base of stack is %p ( %td from main\'s stack )\n",
	    "\t\tmy i address is %p\n"
	    "\t\t\t%td from my stackbase and %td from main\'s\n",	    
	    syscall(SYS_gettid), me,
	    (void*)my_stackbase, (void*)base_of_stack - (void*)my_stackbase,
	    &i, (void*)&i - (void*)my_stackbase, (void*)&i - (void*)base_of_stack);	    
  }

  printf( "\n" );  
  return 0;
}
