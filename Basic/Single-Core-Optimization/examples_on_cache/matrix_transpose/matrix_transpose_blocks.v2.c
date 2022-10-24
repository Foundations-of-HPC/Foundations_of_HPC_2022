#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>


/*
 * -------------------------------------
 *
 *   define types and default values
 *
 * ------------------------------------- 
 */


#if defined( INT8  )                            // use 64bits integers

#warning "using long long integers"
typedef unsigned long long int data_t;

#else                                           // use 32bits integers

typedef unsigned int data_t;
#warning "using integers"

#endif

#define COLUMNS_DFLT 128LLU
#define ROWS_DFLT    128LLU
#define BLOCKSIZE_DFLT 64

typedef unsigned long long int idx_t;


typedef unsigned long long int idx_t;

#define CPU_TIME ({struct timespec ts; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), \
					 (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;})




/*
 * -------------------------------------
 *
 *   main
 *
 * ------------------------------------- 
 */




int main(int argc, char **argv)
{

  int block_size = (argc > 1 ? atoi(*(argv+1)) : BLOCKSIZE_DFLT );   // get the numbr of columns
  int nrows      = (argc > 2 ? atoi(*(argv+2)) : ROWS_DFLT );        // get the number of rows
  int ncols      = (argc > 3 ? atoi(*(argv+3)) : COLUMNS_DFLT );     // get the numbr of columns
  int check      = (argc > 4 ? atoi(*(argv+4)) : 0 );                // about cheking the final matrix
  

  /*
   * allocate the memory
   */
  data_t *matrix  = (data_t*) calloc(2 * nrows*ncols, sizeof(data_t)); // the original matrix
  data_t *tmatrix = matrix + (nrows*ncols);                            // the transposed matrix


  int half_datat = (sizeof(data_t)*4);                               // the number of bits for half
								     // of the data type (that is used
								     // to initialize and check the
								     // matrix entries)

  /*
   * generate the original matrix
   */

  for ( idx_t row = 0; row < nrows; row++ )
    {
      //
      // each entry has the values of the pair
      //   row, column
      // stored in the upper and bottom half
      // respectively
      // In this way it will be easy to check
      // the correctness of the transposed matrix
      //
      
      idx_t  offset = row*ncols;
      data_t value = (row << half_datat );

      for ( idx_t col = 0; col < ncols; col++ )
	matrix[ offset + col ] = value | col;      
    }
	  
  /*
   * transpose the matrix
   */

  double timing = CPU_TIME;
  
  idx_t row_nblocks = nrows / block_size - (ncols % block_size==0);
  idx_t row_remind  = (nrows % block_size ? nrows - row_nblocks*block_size : block_size);
  idx_t col_nblocks = ncols / block_size - (ncols % block_size==0);
  idx_t col_remind  = (ncols % block_size ? ncols - col_nblocks*block_size : block_size);


  for ( idx_t rb = 0; rb <= row_nblocks; rb++ )
    {
      idx_t row_start = rb*block_size;
      idx_t row_end   = row_start + ( rb < row_nblocks ? block_size : row_remind );
	
      for ( idx_t cb = 0; cb <= col_nblocks; cb++ )
	{
	  idx_t col_start = cb*block_size;
	  idx_t col_end   = col_start + ( cb < col_nblocks ? block_size : col_remind );

	  for ( idx_t ri = row_start; ri < row_end; ri++ )
	    {
	      idx_t read_offset  = ri*ncols+col_start;
	      idx_t write_offset = col_start*nrows+ri;

	      for( idx_t ci = col_start; ci < col_end; ci++, write_offset+=nrows )
		tmatrix[ write_offset ] = matrix[read_offset++];

	    }

	}
      
    }

  timing = CPU_TIME - timing;

  if ( check )
    {
  
      /*
       * check the transposed matrix
       */

      idx_t faults = 0;
      for ( idx_t col = 0; col < ncols; col++ )
	{
	  //
	  // each entry has the values of the pair
	  //   row, column
	  // stored in the upper and bottom half
	  // respectively
	  // In this way it will be easy to check
	  // the correctness of the transposed matrix
	  //
      
	  idx_t  offset = col*nrows;
	  data_t value  = col ;

	  for ( idx_t row = 0; row < nrows; row++ )
	    faults+= (tmatrix[ offset + row ] != (value | ( row<<half_datat )));
	}

      if( faults )
	printf("%llu faults found!\n", faults);
      else
	printf("the transposed matrix is ok\n");
    }
  
  printf("transpose time is %g\n", timing );  

  free ( matrix );
  
  return 0;
}
