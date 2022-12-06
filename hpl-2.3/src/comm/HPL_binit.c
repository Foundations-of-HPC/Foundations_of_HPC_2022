/* 
 * -- High Performance Computing Linpack Benchmark (HPL)                
 *    HPL - 2.3 - December 2, 2018                          
 *    Antoine P. Petitet                                                
 *    University of Tennessee, Knoxville                                
 *    Innovative Computing Laboratory                                 
 *    (C) Copyright 2000-2008 All Rights Reserved                       
 *                                                                      
 * -- Copyright notice and Licensing terms:                             
 *                                                                      
 * Redistribution  and  use in  source and binary forms, with or without
 * modification, are  permitted provided  that the following  conditions
 * are met:                                                             
 *                                                                      
 * 1. Redistributions  of  source  code  must retain the above copyright
 * notice, this list of conditions and the following disclaimer.        
 *                                                                      
 * 2. Redistributions in binary form must reproduce  the above copyright
 * notice, this list of conditions,  and the following disclaimer in the
 * documentation and/or other materials provided with the distribution. 
 *                                                                      
 * 3. All  advertising  materials  mentioning  features  or  use of this
 * software must display the following acknowledgement:                 
 * This  product  includes  software  developed  at  the  University  of
 * Tennessee, Knoxville, Innovative Computing Laboratory.             
 *                                                                      
 * 4. The name of the  University,  the name of the  Laboratory,  or the
 * names  of  its  contributors  may  not  be used to endorse or promote
 * products  derived   from   this  software  without  specific  written
 * permission.                                                          
 *                                                                      
 * -- Disclaimer:                                                       
 *                                                                      
 * THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ---------------------------------------------------------------------
 */ 
/*
 * Include files
 */
#include "hpl.h"

#ifdef STDC_HEADERS
int HPL_binit
(
   HPL_T_panel *                    PANEL
)
#else
int HPL_binit
( PANEL )
   HPL_T_panel *                    PANEL;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_binit initializes  a  row  broadcast.  Successful  completion  is
 * indicated by the returned error code HPL_SUCCESS.
 *
 * Arguments
 * =========
 *
 * PANEL   (input/output)                HPL_T_panel *
 *         On entry,  PANEL  points to the  current panel data structure
 *         being broadcast.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        ierr;
   HPL_T_TOP                  top;
/* ..
 * .. Executable Statements ..
 */
   if( PANEL->grid->npcol <= 1 ) return( HPL_SUCCESS );
/*
 * Retrieve the selected virtual broadcast topology
 */
   top = PANEL->algo->btopo;

   switch( top )
   {
      case HPL_1RING_M : ierr = HPL_binit_1rinM( PANEL ); break;
      case HPL_1RING   : ierr = HPL_binit_1ring( PANEL ); break;
      case HPL_2RING_M : ierr = HPL_binit_2rinM( PANEL ); break;
      case HPL_2RING   : ierr = HPL_binit_2ring( PANEL ); break;
      case HPL_BLONG_M : ierr = HPL_binit_blonM( PANEL ); break;
      case HPL_BLONG   : ierr = HPL_binit_blong( PANEL ); break;
      default          : ierr = HPL_SUCCESS;
   }
 
   return( ierr );
/*
 * End of HPL_binit
 */
}
