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
#include <time.h>
#include <math.h>
//#include <sys/resource.h>


//  ================================================

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), \
		  (double)ts.tv_sec +				  \
		  (double)ts.tv_nsec * 1e-9)


#define N_DEFAULT 4000
#define ITER      100000


//  ================================================

char * function_1         ( void );

int    function_2         ( char * );

int    function_3         ( char * );


//  ================================================

float *on_heap;                                         // this is a global variable

char  sentence[] = {"The answer is obviously 42\n"};    // this is another global variable

//  ================================================




int main ( int argc, char **argv )
{
#define SENTENCE_LENGTH 30
  int   run;
  char  my_sentence[SENTENCE_LENGTH+2];
  char  *sentence_ptr;
  
  if ( argc > 1 )
    run = (int)atoi( *(argv+1) );

  if ( argc > 2 )
    {
      strncpy ( my_sentence, *(argv+2), SENTENCE_LENGTH );
      my_sentence [ SENTENCE_LENGTH ] = '\n';
      my_sentence [ SENTENCE_LENGTH +1 ] = '\0';
      sentence_ptr = my_sentence;
    }
  else
    sentence_ptr = sentence;
    
  
  printf ( "\nrunning test.. " ); fflush(stdout);
  
  char *func_1_stmt = function_1( );
  
  // this line will end with a not-so-surprising result
  printf ( "function_1()'s statement as seen from main(): %s\n", func_1_stmt);
  
  function_2 ( sentence_ptr );
  
  printf("\n"); 
  
  return 0;
}




char * function_1 ( void )
{

  char my_statement[] = "this is the statement of function_1()\n";
  printf ( "%s", my_statement );

  // returning the address of a local variable
  // will make the compiler warning that you are
  // missing some fundamental point..
  return my_statement;
 #warning ^^ you should get a warning from the compiler at this point, which is intended
}


int function_2 ( char * sentence )
{

  char  local_sentence[] = "there's no sentence in function_2 ()\n";
  char *sentence_ptr = sentence;
  
  if ( sentence == NULL )
    // always remember to check that your pointers are meaningful
    sentence_ptr = local_sentence;
  
  printf ( "function_2() : %s", sentence_ptr );

  // now calling a function passing the address of a
  // local variable does not rise any compiler's concern
  function_3 ( local_sentence );

  return 0;
}



int function_3 ( char * sentence )
{

  if ( sentence != NULL )
    printf ( "function_3() : %s", sentence );
  else
    printf ( "function_3() : I've been told NULL\n" );

  return 0;
}

