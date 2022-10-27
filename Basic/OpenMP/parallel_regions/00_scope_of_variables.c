
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

  register unsigned long long base_of_stack asm("rbp");
  register unsigned long long top_of_stack asm("rsp");

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
#pragma omp parallel private(i)
  {
    long int me = omp_get_thread_num();
    unsigned long long my_stackbase;
    __asm__("mov %%rbp,%0" : "=mr" (my_stackbase));
    
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
