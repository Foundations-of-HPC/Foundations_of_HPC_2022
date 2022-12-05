/*******************************************************************************
* Copyright 1999-2015 Intel Corporation All Rights Reserved.
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#ifndef NOMPI
#include <mpi.h>
#endif
#define MAX(x,y) ((x)<(y)?(y):(x))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define A(x,y)     A[((y)-1)*(lda) + ((x)-1)]
#define B(x,y)     B[((y)-1)*(ldb) + ((x)-1)]
#define C(x,y)     C[((y)-1)*(ldc) + ((x)-1)]
#define D(x,y)     D[((y)-1)*(ldc) + ((x)-1)]

/* Compile for LINUX by default: */
#ifndef LINUX
#define LINUX
#endif

/* Only set one of ACCUR or NOACCUR */
/* ACCUR = Do (limited) accuracy tests */
/* NOACCUR = Skip them */
#ifdef ACCUR
  #ifdef NOACCUR
     #undef NOACCUR
  #endif
#endif

/* Don't do accuracy tests unless the user wants it.  NOACCUR by default: */
/* Note that accuracy are tests are done by comparing DGEMMCALL1 to
   DGEMMCALL2 anyway (presumably from different libraries.  If these are
   the same, which is the way the code is provided, this is a silly test. */
#ifndef ACCUR
  #ifndef NOACCUR
  #define NOACCUR
  #endif
#endif

#define PrintTimeString(x) { \
         time_t __someTime; \
         struct tm *__stime; \
         time(&__someTime); \
         __stime = localtime(&__someTime); \
         printf("\n%s  at %s\n",x,asctime(__stime));\
        } \

#ifdef GEMV
#ifndef ITER
#define ITER 100
#endif
#else
#ifndef ITER
#define ITER 4
#endif
#endif

#ifdef SINGLE
#define PREC float
#endif

#ifndef PREC
#define PREC double
#endif

#ifdef LINUX
#ifndef DCLOCK
#define DCLOCK dsecnd_
#endif
#else
#ifndef DCLOCK
#define DCLOCK DSECND
#endif
#endif

int precbytes = sizeof(PREC);

/* There are two DGEMMCALLS, you can potentially test accuracy if you have
   two different entry points as well.  To compare different DGEMMs with each
   other, compile -DDGEMMCALL1=<first DGEMM> -DDGEMMCALL2=<second DGEMM>.
   The default is just to use the obvious choice in any case. */

#if (precbytes==4) // single precision

  #ifdef LINUX
    #ifndef DGEMMCALL1
       #ifdef GEMV
          #define DGEMMCALL1 sgemv_
       #else
          #define DGEMMCALL1 sgemm_
       #endif
    #endif
    #ifndef DGEMMCALL2
       #ifdef GEMV
          #define DGEMMCALL2 sgemv_
       #else
          #define DGEMMCALL2 sgemm_
       #endif
    #endif
  #else
    #ifndef DGEMMCALL1
       #ifdef GEMV
          #define DGEMMCALL1 SGEMV
       #else
          #define DGEMMCALL1 SGEMM
       #endif
    #endif
    #ifndef DGEMMCALL2
       #ifdef GEMV
          #define DGEMMCALL2 SGEMV
       #else
          #define DGEMMCALL2 SGEMM
       #endif
    #endif
  #endif

#else // double precision

  #ifdef LINUX
    #ifndef DGEMMCALL1
       #ifdef GEMV
          #define DGEMMCALL1 dgemv_
       #else
          #define DGEMMCALL1 dgemm_
       #endif
    #endif
    #ifndef DGEMMCALL2
       #ifdef GEMV
          #define DGEMMCALL2 dgemv_
       #else
          #define DGEMMCALL2 dgemm_
       #endif
    #endif
  #else
    #ifndef DGEMMCALL1
       #ifdef GEMV
          #define DGEMMCALL1 DGEMV
       #else
          #define DGEMMCALL1 DGEMM
       #endif
    #endif
    #ifndef DGEMMCALL2
       #ifdef GEMV
          #define DGEMMCALL2 DGEMV
       #else
          #define DGEMMCALL2 DGEMM
       #endif
    #endif
  #endif

#endif

/* In some cases, one might want to change this code to do lots of different
   runs over various offsets and problem sizes, and record the performance
   data for later analysis */

#define RECORDSPACE 32000

int main(int argc, char **argv)
{
   double recordvals[RECORDSPACE];
   int irecordvals = 0;
   PREC *A, *B, *C, *D;
   char *At, *Bt, *Ct, *Dt;
   int istrana = 0;
   int istranb = 0;
   int m, n, k, lda, ldb, ldc, i, j, nerrs=0, irow, icol, iii;
   signed long ia, ja, ib, jb, ic, jc, malloced;
#ifndef GEMV
   extern void DGEMMCALL1(char *, char *, int *, int *, int *, PREC *, \
	PREC *, int *, PREC *, int *, PREC *, PREC *, int *);
#ifndef NOACCUR
   extern void DGEMMCALL2(char *, char *, int *, int *, int *, PREC *, \
	PREC *, int *, PREC *, int *, PREC *, PREC *, int *);
#endif
#else
   extern void DGEMMCALL1(char *, int *, int *, PREC *, \
	PREC *, int *, PREC *, int *, PREC *, PREC *, int *);
#ifndef NOACCUR
   extern void DGEMMCALL2(char *, int *, int *, PREC *, \
	PREC *, int *, PREC *, int *, PREC *, PREC *, int *);
#endif
#endif
   extern double DCLOCK();
   int iaoff, iboff, icoff, me, size, itmp;
   double t1, t2, t3, sum=0.0, low, high, tmpi, derr, ctmp;
   char *imalloc(signed long, signed long *);
   char transa, transb, *unused_char;
   PREC alpha = -1.0;
   PREC beta = 1.0;
#ifndef NOMPI
   MPI_Status status;
#endif
   const char ctrans='C', ntrans='N', ttrans='T';
   const PREC dzero=0.0;
   const PREC done=1.0;
   const PREC dnone=-1.0;
   int izero = 0, ione = 1, provided;

#ifndef NOMPI
   MPI_Init_thread ( &argc, &argv, MPI_THREAD_MULTIPLE, &provided );
   MPI_Comm_rank(MPI_COMM_WORLD, &me);
   MPI_Comm_size(MPI_COMM_WORLD, &size );
   if ( (provided == MPI_THREAD_MULTIPLE) && (me == 0) )
      printf("Multi-threaded MPI detected\n");
   if ( (provided != MPI_THREAD_MULTIPLE) && (me == 0) )
      printf("No multi-threaded MPI detected (Hybrid may or may not work)\n");
#else
   me = 0;
   size = 1;
#endif

   if ( me == 0 )
   {
      PrintTimeString("The time/date of the run...");
   }

   if ( me == 0 )
   {
   printf("This driver was compiled with:\n\t");
   printf("-DITER=%d ",ITER);
#ifdef LINUX
   printf("-DLINUX ");
#endif
#ifdef NOACCUR
   printf("-DNOACCUR ");
#endif
#ifdef ACCUR
   printf("-DACCUR ");
#endif
#ifdef INTERACT
   printf("-DINTERACT ");
#endif
#ifdef SINGLE
   printf("-DSINGLE ");
#endif
#ifdef TABLE   // In case one wants to add some loops over a range of values
   printf("-DTABLE ");
#endif
#ifdef NOMPI
   printf("-DNOMPI ");
#endif

   if ( precbytes == 4 ) printf("-DPREC=float ");
   else if ( precbytes == 8 ) printf("-DPREC=double ");
   else printf("-DPREC=unknown(%d bytes) ",precbytes);

#ifdef GEMV
   printf("-DGEMV ");
#endif

   printf("\n");
   }

#ifdef INTERACT
   if ( me == 0 )
   {
      printf("Enter m, n, k: ");
      scanf("%d%d%d",&m,&n,&k);
      printf("Enter A offset, B offset, C offset (choose 0 if in doubt): ");
      scanf("%d%d%d",&iaoff,&iboff,&icoff);
#ifndef NOMPI
      if ( size > 1 )
      {
         MPI_Bcast ( &m, 1, MPI_INT, 0, MPI_COMM_WORLD );
         MPI_Bcast ( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );
         MPI_Bcast ( &k, 1, MPI_INT, 0, MPI_COMM_WORLD );
         MPI_Bcast ( &iaoff, 1, MPI_INT, 0, MPI_COMM_WORLD );
         MPI_Bcast ( &iboff, 1, MPI_INT, 0, MPI_COMM_WORLD );
         MPI_Bcast ( &icoff, 1, MPI_INT, 0, MPI_COMM_WORLD );
      }
#endif
   } else {
#ifndef NOMPI
      MPI_Bcast ( &m, 1, MPI_INT, 0, MPI_COMM_WORLD );
      MPI_Bcast ( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );
      MPI_Bcast ( &k, 1, MPI_INT, 0, MPI_COMM_WORLD );
      MPI_Bcast ( &iaoff, 1, MPI_INT, 0, MPI_COMM_WORLD );
      MPI_Bcast ( &iboff, 1, MPI_INT, 0, MPI_COMM_WORLD );
      MPI_Bcast ( &icoff, 1, MPI_INT, 0, MPI_COMM_WORLD );
#endif
   }
#else
   /* We could modify this section to loop over various values as well */
   /* Memory use is slightly more than sizeof(precision)*m*n*1 for NOACCUR
      (2 for ACCUR).   So, for double precision DGEMM testing without
      accuracy, it will use around 1GB per mpi process when m=n=10000.  */
   m = 15000;
   n = 15000;
   k = 192;
   iaoff = 0;
   iboff = 0;
   icoff = 0;
#endif

   /* This code tests up to 4 offsets */
   iaoff = MIN(MAX(iaoff,0),4);
   iboff = MIN(MAX(iboff,0),4);
   icoff = MIN(MAX(icoff,0),4);

   /* Make certain the inputs are sensible: */
   m = MAX(m,1);
   n = MAX(n,1);
   k = MAX(k,1);

   if ( istrana == 0 )
   {
      transa = ntrans;
      lda = m;
      ja = k;
   } else {
      transa = ttrans;
      lda = k;
      ja = m;
   }
   ia = lda;
   if ( istranb == 0 )
   {
      transb = ntrans;
      ldb = k;
      jb = n;
   } else {
      transb = ttrans;
      ldb = n;
      jb = k;
   }
   ib = ldb;
   ldc = m;
   ic = ldc;
   jc = n;

   At = (char *) imalloc(ia*ja*sizeof(PREC)+4*sizeof(PREC),&malloced);
   Bt = (char *) imalloc(ib*jb*sizeof(PREC)+4*sizeof(PREC),&malloced);
   Ct = (char *) imalloc(ic*jc*sizeof(PREC)+4*sizeof(PREC),&malloced);
#ifndef NOACCUR
   Dt = (char *) imalloc(ic*jc*sizeof(PREC)+4*sizeof(PREC),&malloced);
#endif
   unused_char = imalloc ( (signed long) -1, &malloced );
   if ( me == 0) printf("Malloc done.  Used %ld bytes\n",malloced);

   A = (PREC *) &At[0];

   B = (PREC *) &Bt[0];

   C = (PREC *) &Ct[0];

#ifndef NOACCUR
   D = (PREC *) &Dt[0];
#endif

#if defined(_WIN_) || defined(_WIN32) 
   for ( i = 0 ; i < ia*ja + iaoff ; i++ )
	A[i] = 1.0 - 2.0*rand();
   for ( i = 0 ; i < ib*jb + iboff ; i++ )
	B[i] = 1.0 - 2.0*rand();
   for ( i = 0 ; i < ic*jc + icoff ; i++ )
	C[i] = 1.0 - 2.0*rand();
#else
   srand48( (long) 0 );
   for ( i = 0 ; i < ia*ja + iaoff ; i++ )
	A[i] = 1.0 - 2.0*drand48();
   for ( i = 0 ; i < ib*jb + iboff ; i++ )
	B[i] = 1.0 - 2.0*drand48();
   for ( i = 0 ; i < ic*jc + icoff ; i++ )
	C[i] = 1.0 - 2.0*drand48();
#endif

#ifndef NOACCUR
#pragma omp parallel for firstprivate(ic,jc,icoff) private(i) schedule(runtime)
   for ( i = 0 ; i < ic*jc + icoff ; i++ ) D[i] = C[i];
#endif

   A = (PREC *) &A[iaoff];
   B = (PREC *) &B[iboff];
   C = (PREC *) &C[icoff];
#ifndef NOACCUR
   D = (PREC *) &D[icoff];
#endif

   /* This is the operation count.  Change it if using a different algorithm */
#ifndef GEMV
   high = 2.0*(double)m*(double)n*(double)k;
#else
   high = 2.0*(double)m*(double)k;
#endif

   for ( i = 0 ; i < ITER ; i++ )
   {
      tmpi = DCLOCK ();
#ifndef GEMV
      DGEMMCALL1(&transa,&transb,&m,&n,&k,&alpha,A,&lda,B,&ldb,&beta,C,&ldc);
#else
      DGEMMCALL1(&transa,&m,&k,&alpha,A,&lda,B,&ione,&beta,C,&ione);
#endif
      tmpi = DCLOCK ()-tmpi;
      if ( i==0 || tmpi < t1 ) t1 = tmpi;
   }

   /* Do an accuracy check if desired: convenient if comparing two different libraries, otherwise skip this. */
#ifndef NOACCUR
   for ( i = 0 ; i < ITER ; i++ )
   {
      tmpi = DCLOCK ();
#ifndef GEMV
      DGEMMCALL2(&transa,&transb,&m,&n,&k,&alpha,A,&lda,B,&ldb,&beta,D,&ldc);
#else
      DGEMMCALL2(&transa,&m,&k,&alpha,A,&lda,B,&ione,&beta,D,&ione);
#endif
      tmpi = DCLOCK ()-tmpi;
      if ( i==0 || tmpi < t2 ) t2 = tmpi;
   }
   for ( j = 1, nerrs=0, derr=0.0 ; j <= n ; j++ )
   {
      for ( i = 1 ; i <= m ; i++ )
      {
         if ( C(i,j) >= D(i,j) )
         {
            ctmp = C(i,j) - D(i,j);
         } else {
            ctmp = D(i,j) - C(i,j);
         }
         derr += ctmp;
#ifdef SINGLE
         if ( fabs(ctmp) > 0.00005 )
#else
         if ( fabs(ctmp) > 0.000000001 )
#endif
         {
            if ( (++nerrs<10) && (j<=10) ) {
               printf("Node %d: Error at C(%d,%d): Act=%g Comp=%g Err=%g Tot=%g\n",
                        me,i,j,D(i,j),C(i,j),ctmp,derr);
            }
         }
      }
   }
#endif

   /* Output Performance Results */
   for ( i = 0 ; i < size ; i++ )
   {
      /* Stall for a quarter second: */
      tmpi = DCLOCK(); while ( DCLOCK() - tmpi < 0.25 );
#ifndef NOMPI
      if ( (me == 0) && (size > 1) && (i != 0) )
      {
         MPI_Send ( &itmp, 0, MPI_BYTE, i, i, MPI_COMM_WORLD );
      }
      if ( (me == i) && (size > 1) && (i != 0) )
      {
         MPI_Recv ( &itmp, 0, MPI_BYTE, 0, i, MPI_COMM_WORLD, &status );
      }
#endif
      /* Print out the outputs */
      if ( me == i )
      {
         fflush(NULL);
         printf("(%d of %d): ",me,size);
         /* The last number below is the Mflops */
#ifndef TABLE
         printf("%c%c lda=%4d ldb=%4d ldc=%4d %1d %1d %1d %08.3f ",
             transa,transb,lda,ldb,ldc,iaoff,iboff,icoff,high/(1000000.0*t1));
#else
         /* If one wants to create a Table of outputs, one can change the
            format of the output here.  Otherwise, use the other printf */
         printf("%c%c %05.2f %05.2f %4d %4d %4d %08.3f ",
		   transa,transb,alpha,beta,m,n,k,high/(1000000.0*t1));
#endif
         fflush(NULL);
#ifndef NOACCUR
         printf("LIB2: %08.3f Number of errors=%d Total Error detected=%g ",
                high/(1000000.0*t2),nerrs,derr);
         fflush(NULL);
#endif
#ifndef NOMPI
         char hostname[MPI_MAX_PROCESSOR_NAME];
         int  namelen;

         if ( MPI_Get_processor_name ( hostname, &namelen ) == MPI_SUCCESS )
             printf("%-.*s\n", namelen, hostname );
#else
         /* Insert anything here that will help you to identify the machines */
         // system("hostname");
         printf("\n");
#endif
  
         fflush(NULL);
      }

   } /* i loop over nodes outputing results */

   fflush(NULL); fflush(stdout); fflush(stderr);
   recordvals[irecordvals++] = high/(1000000.0*t1);
   if ( irecordvals >= RECORDSPACE )
   {
      fprintf(stderr,"Ran out of buffer space.  Exiting\n");
      exit(-1);
   }

#ifndef NOACCUR
   free(Dt);
#endif
   free(Ct);
   free(Bt);
   free(At);

   /* If you want to loop over various inputs, put the completion to the loops
      here and then one can look over the recordvals to find min/max/etc. */

#ifndef NOMPI
   MPI_Finalize();
#endif
}

/* This is just a wrapper around malloc.  The user may substitute anything
   that makes sense here.  This version adds up the total memory used. */

char *imalloc(signed long size, signed long *total)
{
        char *tmp;
        static signed long times = 0, bytes= 0;

        ++times;
        if ( size <= 0 ) {
           *total = bytes;
           bytes = 0;
           return(NULL);
        } else {
           bytes += size;
           tmp= (char *) malloc(size);
           //tmp= (char *) mkl_malloc ( size , 4096 );
           if (tmp==NULL) {
              printf("Failed in the malloc for time %ld\n",times);
              fprintf(stderr,"Could not malloc %ld bytes\n",size);
#ifndef NOMPI
              MPI_Finalize();
#endif
              exit(-1);
           }
           return(tmp);
        }
}

