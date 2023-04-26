

/* =============================================================================
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/*
 * +-------------------------------------+
 * |                                     |
 * |  defines section                    |
 * +-------------------------------------+
 * |                                     |
 * |  insert some comments about defines |
 * |  here                               |
 * +-------------------------------------+
 */

#define DEFAULT_ARG1 1
#define MAX_LEVEL    4


/*
 * +-------------------------------------+
 * |                                     |
 * |  function declaration section       |
 * +-------------------------------------+
 * |                                     |
 * |  insert some useful comments about  |
 * |  declared functions here            |
 * |                                     |
 * |  These functions will be used to    |
 * |  show gdb capabilities about break- |
 * |  pointing, watching, backtracing.   |
 * +-------------------------------------+
 */

int function_1( int );                   // this is used for breakpointing
int function_2( char*, int );            // <--+
int function_3( char*, int );            //    | these are used for backtracing and watching
int function_4( char*, int );            // <--+ 


/*
 * +-------------------------------------+
 * |                                     |
 * |  global variables section           |
 * +-------------------------------------+
 * |                                     |
 * |  insert some comments about vars    |
 * |  here                               |
 * +-------------------------------------+
 */

int level = 0;                           // declare and initialize



/*
 * +-------------------------------------+
 * |                                     |
 * |  function definition section        |
 * +-------------------------------------+
 * |                                     |
 * |  you may have something to say about|
 * |  functions here                     |
 * +-------------------------------------+
 */

int function_1( int arg1 )
// you may want to comment about this function
// for instance clarifying the meaning of
// each argument and of the returned value
//
// arg1 :: defines the first arg
//         allowed ranges are [0 .. 4]
//         expected behvaviour for positive out-of-range:
//         received arg is mapped in range by mod operation
//         expected behvaviour for negative out-of-range:
//         an error code is returned
// ret  :: it is the result of the calls stack
//         negative values indicate pathological
//         result:
//         -1 non valid arg1
//         -2 extremely stupid arg1
//         -3 I need a coffee
// 
{
#define MAX_VALUE  5
  int i;
  char buffer[100] = "\0";

  // set-up the indentation and signatures
  for ( i = 0; (i < 98) && (i < MAX_LEVEL); i++ )
    buffer[ i ] = ' ';
  buffer[ i ] = '\0';


  buffer[++level] = '.';                     // increase level and add signature
                                             // add correct indentation to buffer
  int pos = strlen( buffer );
  if ( pos < 98 )                            // you should *ALWAYS* be sure not to overflow
    sprintf( &buffer[pos], "%s", "\t" );
  

  // just a greeting
  printf( "%s the function %s receives %d\n", buffer, __FUNCTION__, arg1 );

  // check the argument
  if ( arg1 < 0 )
    {
      switch( arg1 )
	{
	case -1: return -2;                  // don't need the break statement
	case -2: return -1;
	default: return -3;
	}      
    }
  if ( arg1 >= MAX_VALUE )
    i = i % MAX_VALUE;

  i = arg1*2;

  printf( "%s now it starts a call stack that you can follow: "
	  "the expected return value is %d\n",
	  buffer, ((i | 16) | 32)| 64 );

  // erase signature of this level
  buffer[level] = ' ';
  
  // call the first function
  int ret = function_2( buffer, i );

  // add signature of this level
  buffer[level] = '.';
  
  printf( "\n%s it obtained the value: %d\n", buffer, ret);

  // delete the last tab; well, basically pointless since
  // the variable buffer will not exist anymore after return
  buffer[pos]     = '\0';
  level--;
  
  // return the obtained value
  return ret;
#undef MAX_VALUE
}



int function_2( char *buffer, int arg1 )
//
// This function receives an integer as argument
// and returns the same integer with the 4th bit
// flagged.
// Therefore, allowed arg ranges are all ints living
// in [0..15];
//
// buffer :: a string that is printed before this
//           function's messages; the scope is to
//           guarantee the correct indentation.
//           A tab is added before passing it to
//           the next function.
// arg1   :: the integer argument
//           allowed ranges are [0 .. 15]
//           expected behvaviour for positive out-of-range:
//           received arg is mapped in range by mod operation
//           expected behvaviour for negative out-of-range:
//           returns an error code
// ret    :: the integer arg with the 4th bit flagged
//           negative values indicate pathological
//           result:
//            -1 non valid arg1
//            -2 extremely stupid arg1
//            -3 I need a coffee
// 
{
#define MAX_VALUE  16

  int i;

  buffer[++level] = '.';                     // increase level and add signature
                                             // add correct indentation to buffer
  int pos = strlen( buffer );
  if ( pos < 98 )                            // you should *ALWAYS* be sure not to overflow
    sprintf( &buffer[pos], "%s", "\t" );
  
  printf( "%s the function %s receives %d\n", buffer, __FUNCTION__, arg1 );

  // check the argument
  if ( arg1 < 0 )
    {
      switch( arg1 )
	{
	case -1: return -2;   // don't need the break statement
	case -2: return -1;
	default: return -3;
	}      
    }
  if ( arg1 >= MAX_VALUE )
    i = i % MAX_VALUE;

  i = arg1 | 16;
  
  printf( "%s now it continues the call stack that you can follow\n", buffer);

  // erase signature of this level
  buffer[level]   = ' ';
  
  // call the next function
  int ret = function_3( buffer, i );
  
  // delete the last tab
  buffer[pos]     = '\0';                     // erase identation

  // decrease level
  level--;

  // return the obtained value
  return ret;
#undef MAX_VALUE  
}



int function_3( char *buffer, int arg1 )
//
// This function receives an integer as argument
// and returns the same integer with the 5th bit
// flagged.
// Therefore, allowed arg ranges are all ints living
// in [0..31];
//
// buffer :: a string that is printed before this
//           function's messages; the scope is to
//           guarantee the correct indentation.
//           A tab is added before passing it to
//           the next function.
// arg1   :: the integer argument
//           allowed ranges are [0 .. 31]
//           expected behvaviour for positive out-of-range:
//           received arg is mapped in range by mod operation
//           expected behvaviour for negative out-of-range:
//           returns an error code
// ret    :: the integer arg with the 5th bit flagged
//           negative values indicate pathological
//           result:
//            -1 non valid arg1
//            -2 extremely stupid arg1
//            -3 I need a coffee
// 
{
#define MAX_VALUE  32

  int i;

  buffer[++level] = '.';                     // increase level and add signature
                                             // add correct indentation to buffer
  int pos = strlen( buffer );
  if ( pos < 98 )                            // you should *ALWAYS* be sure not to overflow
    sprintf( &buffer[pos], "%s", "\t" );
  
  printf( "%s the function %s receives %d\n", buffer, __FUNCTION__, arg1 );

  // check the argument
  if ( arg1 < 0 )
    {
      switch( arg1 )
	{
	case -1: return -2;
	case -2: return -1;
	default: return -3;
	}      
    }
  if ( arg1 >= MAX_VALUE )
    i = i % MAX_VALUE;

  i = arg1 | 32;
  
  printf( "%s now it continues the calls stack that you can follow\n", buffer );

  // erase signature of this level
  buffer[level]   = ' ';                      
  // call the next function
  int ret = function_4( buffer, i );

  
  // decrease level
  level--;

  // return the obtained value
  return ret;
#undef MAX_VALUE	 
}




int function_4( char *buffer, int arg1 )
//
// This function receives an integer as argument
// and returns the same integer with the 6th bit
// flagged.
// Therefore, allowed arg ranges are all ints living
// in [0..63];
//
// buffer :: a string that is printed before this
//           function's messages; the scope is to
//           guarantee the correct indentation.
//           A tab is added before passing it to
//           the next function.
// arg1   :: the integer argument
//           allowed ranges are [0 .. 63]
//           expected behvaviour for positive out-of-range:
//           received arg is mapped in range by mod operation
//           expected behvaviour for negative out-of-range:
//           returns an error code
// ret    :: the integer arg with the 5th bit flagged
//           negative values indicate pathological
//           result:
//            -1 non valid arg1
//            -2 extremely stupid arg1
//            -3 I need a coffee
// 
{
#define MAX_VALUE  64

  int i;

  buffer[++level] = '.';                     // increase level and add signature
                                             // add correct indentation to buffer
  int pos = strlen( buffer );
  if ( pos < 98 )                            // you should *ALWAYS* be sure not to overflow
    sprintf( &buffer[pos], "%s", "\t" );
  
  printf( "%s the function %s receives %d\n", buffer, __FUNCTION__,  arg1 );

  // check the argument
  if ( arg1 < 0 )
    {
      switch( arg1 )
	{
	case -1: return -2;   // don't need the break statement
	case -2: return -1;
	default: return -3;
	}      
    }
  if ( arg1 >= MAX_VALUE )
    i = i % MAX_VALUE;

  i = arg1 | 64;

  printf( "%s now it gets back\n", buffer );

  // erase signature of this level
  buffer[level]   = ' ';                      

  // delete the last tab
  buffer[pos]     = '\0';                     // erase identation

  level--;
  
  // return the obtained value
  return i;
#undef MAX_VALUE
}




/*
 * +-------------------------------------+
 * |                                     |
 * |  main                               |
 * +-------------------------------------+
 */

int main ( int argc, char **argv )
{

  if ( argc > 1)
    // arg 0 is the name of the program itself
    {
      printf( "\nexploring my %d argument%c:\n", argc-1, (argc>2)?'s':' ' );
      for ( int i = 1; i < argc; i++ )       
	printf( "\targument %d is : %s\n", i, *(argv+i) );
      
      printf( "\n" );
    }
  
  else
    
    printf ( "no arguments were given, using default: %d\n\n", DEFAULT_ARG1 );


  int arg1;
  
  if ( argc > 1 )
    arg1 = atoi( *(argv+1) );

  else
    arg1 = DEFAULT_ARG1;

  int ret;
  
  ret = function_1( arg1 );
  printf( "main has receveid a return value of %d: %s\n", ret,
	  ( ret >= 0 )? "looks ok" : "something pathological happened" );

  return ret;
}
