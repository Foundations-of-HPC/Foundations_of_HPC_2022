
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

typedef struct
{
  char   tag_for_this_data[24];
  double field_of_doubles[12];
} data_t;

char   this_string_goes_in_initialized_global_data[] = "Hello World!\n";
double this_double_goes_in_initialized_global_data   = 3.1415926535897932;

int    function1 ( int, int, char *, data_t );
  
int main ( int argc, char **argv )
{

  register long long int myRBP __asm__("rbp");   // this way you can directly access a CPU register
  register long long int myRSP __asm__("rsp");   // BP is the Base of the Stack and SP is the Top of the Stack
						 //
  
  printf("------------------------------\n"
	 "%s\n\n"
	 "My RBP points to %p "
	 "My RSP points to %p "
	 " ( stack size: %zd )\n",
	 __FUNCTION__, (void*)myRBP, (void*)myRSP, (void*)myRBP - (void*)myRSP );

  printf("\targc resides at %p [ %zd bytes-offset from RSP]\n",
	 &argc,
	 (void*)&argc - (void*)myRSP);
	 
  for ( int ii = 0; ii < argc; ii++ )
    {
      printf("\targ%d's pointer resides at %p [ %zd bytes-offset from RSP]\n"
	     "\targ%d resides at %p [ %zd bytes-offset from RSP]\n\n",
	     ii,
	     argv + ii,
	     (void*)(argv+ii) - (void*)myRSP,
	     ii, *(argv+ii),
	     (void*)*(argv+ii) - (void*)myRSP);
    }

  int    add_4bytes_to_stack;
  double add_8bytes_to_stack;
  data_t my_data = {"this is an example", {1.0, 2.0, 3.0} };
  
  printf("after the local variables (*):\n"
	 "My RBP points to %p\n"
	 "My RSP points to %p\n"
	 "\n(*) the stack has not changed because the compiler\n"
	 "    already knew about these latter variales\n\n",
	 (void*)myRBP, (void*)myRSP );

  printf("\tadd_4bytes_to_stack resides at %p [ %zd bytes-offset from RSP]\n"
	 "\tadd_8bytes_to_stack resides at %p [ %zd bytes-offset from RSP]\n"
	 "\tmy_data resides at %p [ %zd bytes-offset from RSP]\n",
	 &add_4bytes_to_stack,
	 (void*)&add_4bytes_to_stack - (void*)myRSP,
	 &add_8bytes_to_stack,
	 (void*)&add_8bytes_to_stack - (void*)myRSP,
	 (void*)&my_data,
	 (void*)&my_data - (void*)myRSP);

  function1( 1, 2, this_string_goes_in_initialized_global_data, my_data );
  
  return 0;
}


int function1( int iarg1, int iarg2, char *sarg3, data_t data )
//
// n.b. just get the point.. the exact placement in registers is
//      compiler dependent
//
{

  register long long int myRBP __asm__("rbp");
  register long long int myRSP __asm__("rsp");
  register long long int myRDI __asm__("rdi");
  register long long int myRSI __asm__("rsi");
  register long long int myRDX __asm__("rdx");
  
  int    add_4bytes_to_stack;

  printf("------------------------------\n"
	 "%s\n\n"
	 "RBP points to %p, RSP points to %p (stack size: %zd)\n"
	 "\tdata (%lu bytes long) resides at %p [ %zd bytes-offset from RSP]\n"
	 "\t >>>  note that the argument \"data\" seems to be in between the main's stack and the function1's stack <<<\n"
	 "\tadd_4bytes_to_stack resides at %p [ %zd bytes-offset from RSP]\n"	 
	 "\t RDI register value is %lld\n"
	 "\t RSI register value is %lld\n"
	 "\t RDX register value is %p ( sarg3 points to %p)\n",
	 __FUNCTION__,
	 (void*)myRBP, (void*)myRSP, (void*)myRBP - (void*)myRSP,
	 sizeof(data_t), &data,
	 (void*)&data - (void*)myRSP,
	 &add_4bytes_to_stack,
	 (void*)&add_4bytes_to_stack - (void*)myRSP,
	 myRDI, myRSI, (void*)myRDX, this_string_goes_in_initialized_global_data);

  printf("\n\n"
	 "let's explore where data actually is:\n"
	 "\ttag_for_this_data: %p\n"
	 "\tfield_of_doubles[0] : %p\n"
	 "\tfield_of_doubles[1] : %p\n"
	 "\t...\n"
	 "\tfield_of_doubles[11] : %p\n",
	 &data.tag_for_this_data,
	 &data.field_of_doubles[0],
	 &data.field_of_doubles[1],
	 &data.field_of_doubles[11] );
	 
  

  return 0;
}
