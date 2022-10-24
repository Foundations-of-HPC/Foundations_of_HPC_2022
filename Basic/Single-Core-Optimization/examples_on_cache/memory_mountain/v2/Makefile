
COMPILER=gcc

ifeq ($(COMPILER),gcc)
CC = gcc
CFLAGS = -Wall -O3 -march=native -ftree-vectorize -lm
SUFFIX = .gcc
LIBM = -lm
endif

ifeq ($(COMPILER),icc)
CC = icc
CFLAGS = -Wall -O3 -fast -axSSE4.2 -xHost -ipo
SUFFIX = .icc
LIBM =
endif

ifeq ($(COMPILER),pgcc)
CC = pgcc
CFLAGS = -Wall -O4 -fast -Munroll -Mvect=simd,fuse,tile -Mipa -lm
SUFFIX = .pgcc
LIBM = -lm
endif

mountain: mountain.c fcyc.c
	$(CC) $(CFLAGS) -o mountain$(SUFFIX) mountain.c fcyc.c $(LIBM)

clean:
	rm -f mountain$(SUFFIX) *.o *~


