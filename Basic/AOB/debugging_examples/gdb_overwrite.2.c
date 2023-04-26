
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

#define N 4

int main( int argc, char **argv )
{
  double local_array[N] = {0};
  long long int top_stack[N] = {0};
  
  

  printf("addresses are:\n"
	 "top_stack   : %p\n"
	 "local_array : %p\n",
	 top_stack, local_array);

  for( int i = 0; i < N; i++ ) top_stack[i] = i;
  
  for( int i = 0; i <= N; i++ ) {
    printf("writing local_array item %d at %p\n", i, local_array+i);
    local_array[i] = (double)i; }

  printf("new values are:\n");
  
  for ( int i = 0; i < N; i++ )
    printf("top_stack[%d] : %lld\n", i, top_stack[i]);
  
  for ( int i = 0; i < N; i++ )
    printf("local_array[%d] : %g\n", i, local_array[i]);

  return 0;
  
}
