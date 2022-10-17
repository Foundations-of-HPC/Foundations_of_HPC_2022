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

int    access_the_stack   ( void );

int    access_the_stack_dp( void );

int    access_the_heap    ( void );

int    access_the_heap_dp ( void );

char * function_1         ( void );

int    function_2         ( char * );

int    function_3         ( char * );


//  ================================================

float *on_heap;                                         // this is a global variable

char  sentence[] = {"The answer is obviously 42\n"};    // this is another global variable




//  ================================================

int access_the_stack( void )
{
  float  on_stack[N_DEFAULT];
  double time     = 0;
  double std_dev  = 0;
  struct  timespec ts;
  
  for ( int r = 0; r < ITER; r++ )
    {
      double t0 = CPU_TIME;
      
      for ( int i = 0; i < N_DEFAULT; i++ )
	on_stack[ i ] = (float)i;
      float volatile access = 0;
      for ( int i = 0; i < N_DEFAULT; i++ )
	access += on_stack[ i ] + 1.0;

      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }
  
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);
  
  printf("timing for accessing the stack was: %g (sigma: %g)\n\n", time, std_dev);

  return 0;
}


int access_the_stack_dp( void )
{
  float  on_stack[N_DEFAULT];

  double time     = 0;
  double std_dev  = 0;
  struct  timespec ts;

    for ( int r = 0; r < ITER; r++ )
    {
      float  register * ptr;
      const float  register * ptr_end = on_stack + N_DEFAULT;
      double t0 = CPU_TIME;

      for ( ptr = on_stack ; ++ptr < ptr_end; )
	*ptr = (float)(ptr-on_stack);
      float volatile access = 0;      
      for ( ptr = on_stack; ++ptr < ptr_end; )
	access += *ptr + 1.0;

      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }
  
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);
  
  printf("timing for accessing the stack was: %g (sigma: %g)\n\n", time, std_dev);

  return 0;
}


int access_the_heap( void )
{
  double time    = 0;
  double std_dev = 0;
  struct  timespec ts;

  on_heap = (float*)calloc( N_DEFAULT, sizeof(float) );

  for ( int r = 0; r < ITER; r++ )
    {
      double t0 = CPU_TIME;
      
      for ( int i = 0; i < N_DEFAULT; i++ )
	on_heap[ i ] = (float)i;
      float volatile access = 0;
      for ( int i = 0; i < N_DEFAULT; i++ )
	access += on_heap[ i ] + 1.0;

      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }

  free ( on_heap );
  
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);
  
  printf("timing for accessing the heap with ptr arithmetic was: %g (sigma: %g)\n\n", time, std_dev);
  
  return 0;
}


int access_the_heap_dp ( void )
{
  
  double time    = 0;
  double std_dev = 0;
  struct  timespec ts;

  on_heap = (float*)calloc( N_DEFAULT, sizeof(float) );
  
  for ( int r = 0; r < ITER; r++ )
    {
      float register * entry = on_heap;
      const float register * stop  = on_heap + N_DEFAULT;

      double t0 = CPU_TIME;

      for ( ; ++entry < stop;)
	*entry = (float)(entry - on_heap);
      float volatile access = 0;
      for ( entry = on_heap; ++entry < stop;  )
	access += *entry + 1.0;
      
      double timediff = CPU_TIME - t0;
      time    += timediff;
      std_dev += timediff*timediff;
    }

  free ( on_heap );
      
  time /= ITER;
  std_dev = sqrt(std_dev / ITER - time*time);

  printf("timing for accessing the hep with reduced ptr arithmetic was: %g (sigma: %g)\n\n", time, std_dev);

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
  
  if ( run == 0 )
    access_the_stack ( );

  else if ( run == 1 )
    access_the_stack_dp ( );
  
  else if ( run == 2 )
    access_the_heap ( );

  else if ( run == 3 )
    access_the_heap_dp ( );


    
    char *func_1_stmt = function_1( );
    
    // this line will end with a not-so-surprising result
    printf ( "function_1()'s statement as seen from main(): %s\n", func_1_stmt);
    
    function_2 ( sentence_ptr );
    
    printf("\n"); 
  
  return 0;
}
