#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#ifndef NTHREADS
#define NTHREADS     4
#endif

void *ShowUp(void *thread_id)
{
  printf("Thread #%zd says: \" Hello World! \"\n", (size_t)thread_id);
  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NTHREADS];
   int  rc;
   size_t t;
   
   for(t=0; t<NTHREADS; t++)
     {
       
       printf("Creating thread %zd\n", t);
       rc = pthread_create(&threads[t], NULL, ShowUp, (void *)t);
      
       if (rc)
	 {
	   printf("return code from pthread_create() is %d\n", rc);
	   exit(-1);
	 }
   }

   /* Last thing that main() should do */
   pthread_exit(NULL);
}
