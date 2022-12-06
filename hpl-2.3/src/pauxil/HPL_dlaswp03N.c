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
/*
 * Define default value for unrolling factor
 */
#ifndef HPL_LASWP03N_DEPTH
#define    HPL_LASWP03N_DEPTH       32
#define    HPL_LASWP03N_LOG2_DEPTH   5
#endif

#ifdef STDC_HEADERS
void HPL_dlaswp03N
(
   const int                        M,
   const int                        N,
   double *                         U,
   const int                        LDU,
   const double *                   W0,
   const double *                   W,
   const int                        LDW
)
#else
void HPL_dlaswp03N
( M, N, U, LDU, W0, W, LDW )
   const int                        M;
   const int                        N;
   double *                         U;
   const int                        LDU;
   const double *                   W0;
   const double *                   W;
   const int                        LDW;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dlaswp03N copies columns of  W  into  rows  of an  array  U.  The
 * destination in U of these columns contained in W is stored within W0.
 *
 * Arguments
 * =========
 *
 * M       (local input)                 const int
 *         On entry, M  specifies  the  number  of columns of  W  stored
 *         contiguously that should be copied into U. M must be at least
 *         zero.
 *
 * N       (local input)                 const int
 *         On entry,  N  specifies  the  length of columns of  W  stored
 *         contiguously that should be copied into U. N must be at least
 *         zero.
 *
 * U       (local input/output)          double *
 *         On entry, U points to an array of dimension (LDU,N).  Columns
 *         of W are copied as rows within this array U at  the positions
 *         specified in W0.
 *
 * LDU     (local input)                 const int
 *         On entry, LDU specifies the leading dimension of the array U.
 *         LDU must be at least MAX(1,M).
 *
 * W0      (local input)                 const double *
 *         On entry,  W0  is an array of size (M-1)*LDW+1, that contains
 *         the destination offset  in U where the columns of W should be
 *         copied.
 *
 * W       (local input)                 const double *
 *         On entry, W  is an array of size (LDW,M),  that contains data
 *         to be copied into U. For i in [0..M),  entries W(:,i)  should
 *         be copied into the row or column W0(i*LDW) of U.
 *
 * LDW     (local input)                 const int
 *         On entry, LDW specifies the leading dimension of the array W.
 *         LDW must be at least MAX(1,N+1).
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   const double               * w = W, * w0;
   double                     * u0;
   const int                  incU = (int)( (unsigned int)(LDU) <<
                                            HPL_LASWP03N_LOG2_DEPTH );
   int                        nr, nu;
   register int               i, j;
/* ..
 * .. Executable Statements ..
 */
   if( ( M <= 0 ) || ( N <= 0 ) ) return;

   nr = N - ( nu = (int)( ( (unsigned int)(N) >> HPL_LASWP03N_LOG2_DEPTH ) <<
                          HPL_LASWP03N_LOG2_DEPTH ) );

   for( j = 0; j < nu;
        j += HPL_LASWP03N_DEPTH, U += incU, w += HPL_LASWP03N_DEPTH )
   {
      for( i = 0; i < M; i++ )
      {
         u0 = U + (size_t)(*( W0 + (size_t)(i) * (size_t)(LDW) )); 
         w0 = w + (size_t)(i) * (size_t)(LDW);

         *u0 = w0[ 0]; u0 += LDU;
#if ( HPL_LASWP03N_DEPTH >  1 )
         *u0 = w0[ 1]; u0 += LDU;
#endif
#if ( HPL_LASWP03N_DEPTH >  2 )
         *u0 = w0[ 2]; u0 += LDU; *u0 = w0[ 3]; u0 += LDU;
#endif
#if ( HPL_LASWP03N_DEPTH >  4 )
         *u0 = w0[ 4]; u0 += LDU; *u0 = w0[ 5]; u0 += LDU;
         *u0 = w0[ 6]; u0 += LDU; *u0 = w0[ 7]; u0 += LDU;
#endif
#if ( HPL_LASWP03N_DEPTH >  8 )
         *u0 = w0[ 8]; u0 += LDU; *u0 = w0[ 9]; u0 += LDU;
         *u0 = w0[10]; u0 += LDU; *u0 = w0[11]; u0 += LDU;
         *u0 = w0[12]; u0 += LDU; *u0 = w0[13]; u0 += LDU;
         *u0 = w0[14]; u0 += LDU; *u0 = w0[15]; u0 += LDU;
#endif
#if ( HPL_LASWP03N_DEPTH > 16 )
         *u0 = w0[16]; u0 += LDU; *u0 = w0[17]; u0 += LDU;
         *u0 = w0[18]; u0 += LDU; *u0 = w0[19]; u0 += LDU;
         *u0 = w0[20]; u0 += LDU; *u0 = w0[21]; u0 += LDU;
         *u0 = w0[22]; u0 += LDU; *u0 = w0[23]; u0 += LDU;
         *u0 = w0[24]; u0 += LDU; *u0 = w0[25]; u0 += LDU;
         *u0 = w0[26]; u0 += LDU; *u0 = w0[27]; u0 += LDU;
         *u0 = w0[28]; u0 += LDU; *u0 = w0[29]; u0 += LDU;
         *u0 = w0[30]; u0 += LDU; *u0 = w0[31]; u0 += LDU;
#endif
      }
   }

   if( nr )
   {
      for( i = 0; i < M; i++ )
      {
         u0 = U + (size_t)(*( W0 + (size_t)(i) * (size_t)(LDW) )); 
         w0 = w + (size_t)(i) * (size_t)(LDW);
         for( j = 0; j < nr; j++, u0 += LDU ) { *u0 = w0[j]; }
      }
   }
/*
 * End of HPL_dlaswp03N
 */
} 
