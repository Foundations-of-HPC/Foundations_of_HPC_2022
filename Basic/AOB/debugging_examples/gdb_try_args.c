
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


int main ( int argc, char **argv )
{

  if ( argc > 1)
    // remember: arg 0 is the name of the program itself
    {
      
      printf( "\nexploring my %d argument%c:\n", argc-1, (argc>2)?'s':' ' );
      for ( int i = 1; i < argc; i++ )       
	{
	  printf( "\targument %d is : %s\n", i, *(argv+i) );
	}
      printf( "\n" );
	
    }
  
  else
    
    printf ( "no arguments were given\n\n" );

  return 0;
}
