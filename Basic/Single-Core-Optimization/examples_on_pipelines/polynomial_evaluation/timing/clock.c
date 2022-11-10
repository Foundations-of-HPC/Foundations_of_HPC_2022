#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include "clock.h"


/* Routines for using cycle counter */

/* Detect whether running on Alpha */
#ifdef __alpha
#define IS_ALPHA 1
#else
#define IS_ALPHA 0
#endif

/* Detect whether running on x86 */
#if defined(__i386__) || defined(__x86_64__)
#define IS_x86 1
#else
#define IS_x86 0
#endif




/* Keep track of most recent reading of cycle counter */ 
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

/* Keep track of most recent reading of cycle counter */ 
static unsigned cyc_hi_cp = 0;
static unsigned cyc_lo_cp = 0;

#if IS_ALPHA
/* Use Alpha cycle timer to compute cycles.  Then use
   measured clock speed to compute seconds 
*/

/*
 * counterRoutine is an array of Alpha instructions to access 
 * the Alpha's processor cycle counter. It uses the rpcc 
 * instruction to access the counter. This 64 bit register is 
 * divided into two parts. The lower 32 bits are the cycles 
 * used by the current process. The upper 32 bits are wall 
 * clock cycles. These instructions read the counter, and 
 * convert the lower 32 bits into an unsigned int - this is the 
 * user space counter value.
 * NOTE: The counter has a very limited time span. With a 
 * 450MhZ clock the counter can time things for about 9 
 * seconds. */
static unsigned int counterRoutine[] =
{
 0x601fc000u,
 0x401f0000u,
 0x6bfa8001u
};

/* Cast the above instructions into a function. */
static unsigned int (*counter)(void)= (void *)counterRoutine;


void start_counter()
{
  /* Get cycle counter */
  cyc_hi = 0;
  cyc_lo = counter();
}

double get_counter()
{
  unsigned ncyc_hi, ncyc_lo;
  unsigned hi, lo, borrow;
  double result;
  ncyc_lo = counter();
  ncyc_hi = 0;
  lo = ncyc_lo - cyc_lo;
  borrow = lo > ncyc_lo;
  hi = ncyc_hi - cyc_hi - borrow;
  result = (double) hi * (1 << 30) * 4 + lo;
  if (result < 0) {
    fprintf(stderr, "Error: Cycle counter returning negative value: %.0f\n", result);
  }
  return result;
}
#endif /* Alpha */

#if IS_x86
void access_counter(unsigned *hi, unsigned *lo)
{
  /* Get cycle counter */
  asm("rdtsc; movl %%edx,%0; movl %%eax,%1" 
      : "=r" (*hi), "=r" (*lo)
      : /* No input */ 
      : "%edx", "%eax");
}

void start_counter()
{
  access_counter(&cyc_hi, &cyc_lo);
}

double get_counter()
{
  unsigned ncyc_hi, ncyc_lo;
  unsigned hi, lo, borrow;
  double result;
  /* Get cycle counter */
  access_counter(&ncyc_hi, &ncyc_lo);
  /* Do double precision subtraction */
  lo = ncyc_lo - cyc_lo;
  borrow = lo > ncyc_lo;
  hi = ncyc_hi - cyc_hi - borrow;
  result = (double) hi * (1 << 30) * 4 + lo;
  if (result < 0) {
    fprintf(stderr, "Error: Cycle counter returning negative value: %.0f\n", result);
  }
  return result;
}

void start_counter_copy()
{
  access_counter(&cyc_hi_cp, &cyc_lo_cp);
}

double get_counter_copy()
{
  unsigned ncyc_hi, ncyc_lo;
  unsigned hi, lo, borrow;
  double result;
  /* Get cycle counter */
  access_counter(&ncyc_hi, &ncyc_lo);
  /* Do double precision subtraction */
  lo = ncyc_lo - cyc_lo_cp;
  borrow = lo > ncyc_lo;
  hi = ncyc_hi - cyc_hi_cp - borrow;
  result = (double) hi * (1 << 30) * 4 + lo;
  if (result < 0) {
    fprintf(stderr, "Error: Cycle counter returning negative value: %.0f\n", result);
  }
  return result;
}
#endif /* x86 */

double ovhd()
{
  /* Do it twice to eliminate cache effects */
  int i;
  double result;
  for (i = 0; i < 2; i++) {
    start_counter();
    result = get_counter();
  }
  return result;
}

/* Determine clock rate by measuring cycles
   elapsed while sleeping for sleeptime seconds */
double mhz_full(int verbose, int sleeptime)
{
  double rate;
  start_counter();
  sleep(sleeptime);
  rate = get_counter()/(1e6*sleeptime);
  if (verbose) 
    printf("Processor Clock Rate ~= %.1f MHz\n", rate);
  return rate;
}

/* Version using a default sleeptime */
double mhz(int verbose)
{
  return mhz_full(verbose, 2);
}

/** Special counters that compensate for timer interrupt overhead */

static double cyc_per_tick = 0.0;

#define NEVENT 100
#define THRESHOLD 100  // 1000
#define RECORDTHRESH 500  // 3000

/* Attempt to see how much time is used by timer interrupt */
static void callibrate(int verbose)
{
  struct tms t;
  double oldt;  
  clock_t oldc;
  int e = 0;
  
  times(&t);
  oldc = t.tms_utime;
  
  start_counter();
  oldt = get_counter();
  
  while (e < NEVENT)
    {
      double newt = get_counter();
      
      if (newt-oldt >= THRESHOLD)
	{
	  clock_t newc;
	  times(&t);
	  newc = t.tms_utime;

	  if (newc > oldc)
	    {
	      double cpt = (newt-oldt)/(newc-oldc);
	      if (verbose)
		printf( "Saw event lasting %.0f cycles and %d ticks.  Ratio = %f ",
		       newt-oldt, (int) (newc-oldc), cpt);

	      if ((cyc_per_tick == 0.0 || cyc_per_tick > cpt) && cpt > RECORDTHRESH)
		{
		  cyc_per_tick = cpt;
		  if (verbose)
		    printf( "   --> cyc_per_tick set\n" );
		}
	      else
		printf( "\n" );


	      e++;
	      oldc = newc;
	    }
	  
	  oldt = newt;
	}
    }
  
  if (verbose)
    printf("Setting cyc_per_tick to %f\n", cyc_per_tick);
}

static clock_t start_tick = 0;

void start_comp_counter() {
  // WARNING: this assume that you already calibrate the cyc_per_tick using callibrate()
  struct tms t;
  if (cyc_per_tick == 0.0)
    callibrate(1);

  times(&t);
  start_tick = t.tms_utime;
  start_counter();
}

double get_comp_counter() {
  double time = get_counter();
  double ctime;
  struct tms t;
  clock_t ticks;
  times(&t);
  ticks = t.tms_utime - start_tick;
  ctime = time - ticks*cyc_per_tick;
  /*
  printf("Measured %.0f cycles.  Ticks = %d.  Corrected %.0f cycles\n",
	 time, (int) ticks, ctime);
  */
  return ctime;
}

