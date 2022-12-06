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
void HPL_min
(
   const int                        N,
   const void *                     IN,
   void *                           INOUT,
   const HPL_T_TYPE                 DTYPE
)
#else
void HPL_min
( N, IN, INOUT, DTYPE )
   const int                        N;
   const void *                     IN;
   void *                           INOUT;
   const HPL_T_TYPE                 DTYPE;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_min combines (min) two buffers.
 * 
 *
 * Arguments
 * =========
 *
 * N       (input)                       const int
 *         On entry, N  specifies  the  length  of  the  buffers  to  be
 *         combined. N must be at least zero.
 *
 * IN      (input)                       const void *
 *         On entry, IN points to the input-only buffer to be combined.
 *
 * INOUT   (input/output)                void *
 *         On entry, INOUT  points  to  the  input-output  buffer  to be
 *         combined.  On exit,  the  entries of this array contains  the
 *         combined results.
 *
 * DTYPE   (input)                       const HPL_T_TYPE
 *         On entry,  DTYPE  specifies the type of the buffers operands.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   register int               i;
/* ..
 * .. Executable Statements ..
 */
   if( DTYPE == HPL_INT )
   {
      const int       * a = (const int *)(IN);
      int             * b = (int *)(INOUT);
      for( i = 0; i < N; i++ ) b[i] = Mmin( a[i], b[i] );
   }
   else
   {
      const double    * a = (const double *)(IN);
      double          * b = (double *)(INOUT);
      for( i = 0; i < N; i++ ) b[i] = Mmin( a[i], b[i] );
   }
/*
 * End of HPL_min
 */
}
