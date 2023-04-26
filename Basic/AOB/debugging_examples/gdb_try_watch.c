#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



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

#define DEFAULT_ARG1 100
#define PI           3.14159265358979323846

#define CHECK_VALUE( V, P ) if ( (V) != ctrl_value ) printf("something wrong at point %d\n", P)

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

void function_1( int, double* );         // this function will actually write in memory



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

void function_1( int N, double *array )
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
  int    i;
  double N_inv = 1.0 / N;
  
  for ( i = 0; i < 2*N; i++ )
    array[ i ] = sin( i * N_inv * PI );
    
  return;
}




/*
 * +-------------------------------------+
 * |                                     |
 * |  main                               |
 * +-------------------------------------+
 */

int main ( int argc, char **argv )
{
  int N = DEFAULT_ARG1;

  if ( argc > 1)
    N = atoi( *(argv + 1) );

  else
    printf ( "no arguments were given, using default: %d\n\n", DEFAULT_ARG1 );

  int ret;
  double * darray1 = (double*)calloc( N, sizeof(double) );
  double * darray2 = (double*)calloc( N, sizeof(double) );
  double ctrl_value = sin( 0 );
  
  
  function_1( N, darray2 );

  CHECK_VALUE( darray2[0], 1 );

  function_1( N, darray1 );

  CHECK_VALUE( darray2[0], 2 );
  
  free( darray2 );
  free( darray1 );
  
  return ret;
}
