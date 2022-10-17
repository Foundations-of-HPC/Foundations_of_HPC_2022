
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */



#include<stdlib.h>
#include<stdio.h>
#include<string.h>


typedef struct
{
  char    char_field;
  double  double_field;
  int     int_field;
  char    char_field2;
} STRUCT_A;

typedef struct
{
  char    char_field;
  double  double_field __attribute__ ((packed));
  int     int_field;
  char    char_field2;
} STRUCT_Ap;

typedef struct
{
  char    char_field;
  double  double_field __attribute__ ((packed));
  int     int_field __attribute__ ((packed));
  char    char_field2;
} STRUCT_App;


typedef struct
{
  char    char_field;
  double  double_field;
  char    char_field2;
  int     int_field;
} STRUCT_B;

typedef struct
{
  char    char_field;
  char    char_field2;
  int     int_field;
  double  double_field;
} STRUCT_C;


typedef struct
{
  double  double_field;
  int     int_field;
  char    char_field;
  char    char_field2;
} STRUCT_D;



void main(int argc, char **argv)
{
  STRUCT_A   arrayA[2];
  STRUCT_Ap  arrayAp[2];
  STRUCT_App arrayApp[2];
  STRUCT_B   arrayB[2];
  STRUCT_C   arrayC[2];
  STRUCT_D   arrayD[2];

  int        i, mode;

  if(argc < 2)
    mode = 0;
  else
    mode = 1;
  
  printf("\n"
	 "arrayA begins at %p, size of STRUCT_A is %zd, memory displacement among array elements is %zd\n",
	 arrayA, sizeof(STRUCT_A), (void*)&arrayA[1] - (void*)&arrayA[0]);
  printf("\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\n",
	 "char_field at ",   (void*)&arrayA[0].char_field   - (void*)&arrayA[0],
	 "double_field at ", (void*)&arrayA[0].double_field - (void*)&arrayA[0],
	 "int_field at ",    (void*)&arrayA[0].int_field    - (void*)&arrayA[0],
	 "char_field2 at ",  (void*)&arrayA[0].char_field2  - (void*)&arrayA[0]);
	 

  if(mode)
    {
      printf("\n"
	     "arrayAp begins at %p, size of STRUCT_Ap is %zd, memory displacement among array elements is %zd\n",
	     arrayAp, sizeof(STRUCT_Ap), (void*)&arrayAp[1] - (void*)&arrayAp[0]);
      printf("\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\n",
	     "char_field at ",   (void*)&arrayAp[0].char_field   - (void*)&arrayAp[0],
	     "double_field at ", (void*)&arrayAp[0].double_field - (void*)&arrayAp[0],
	     "int_field at ",    (void*)&arrayAp[0].int_field    - (void*)&arrayAp[0],
	     "char_field2 at ",  (void*)&arrayAp[0].char_field2  - (void*)&arrayAp[0]);
      
      printf("\n"
	     "arrayApp begins at %p, size of STRUCT_Ap is %zd, memory displacement among array elements is %zd\n",
	     arrayApp, sizeof(STRUCT_App), (void*)&arrayApp[1] - (void*)&arrayApp[0]);
      printf("\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\n",
	     "char_field at ",   (void*)&arrayApp[0].char_field   - (void*)&arrayApp[0],
	     "double_field at ", (void*)&arrayApp[0].double_field - (void*)&arrayApp[0],
	     "int_field at ",    (void*)&arrayApp[0].int_field    - (void*)&arrayApp[0],
	     "char_field2 at ",  (void*)&arrayApp[0].char_field2  - (void*)&arrayApp[0]);      
    }
  
  printf("arrayB begins at %p, size of STRUCT_B is %zd, memory displacement among array elements is %zd\n",
	 arrayB, sizeof(STRUCT_B), (void*)&arrayB[1] - (void*)&arrayB[0]);
  printf("\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\n",
	 "char_field at ",   (void*)&arrayB[0].char_field   - (void*)&arrayB[0],
	 "double_field at ", (void*)&arrayB[0].double_field - (void*)&arrayB[0],
	 "char_field2 at ",  (void*)&arrayB[0].char_field2  - (void*)&arrayB[0],
	 "int_field at ",    (void*)&arrayB[0].int_field    - (void*)&arrayB[0]);

  
  printf("arrayC begins at %p, size of STRUCT_C is %zd, memory displacement among array elements is %zd\n",
	 arrayC, sizeof(STRUCT_C), (void*)&arrayC[1] - (void*)&arrayC[0]);
  printf("\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\n",
	 "char_field at ",   (void*)&arrayC[0].char_field   - (void*)&arrayC[0],
	 "char_field2 at ",  (void*)&arrayC[0].char_field2  - (void*)&arrayC[0],
	 "int_field at ",    (void*)&arrayC[0].int_field    - (void*)&arrayC[0],
	 "double_field at ", (void*)&arrayC[0].double_field - (void*)&arrayC[0]);


  printf("arrayD begins at %p, size of STRUCT_D is %zd, memory displacement among array elements is %zd\n",
	 arrayD, sizeof(STRUCT_D), (void*)&arrayD[1] - (void*)&arrayD[0]);
  printf("\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\t%18s%3zd\n\n",
	 "double_field at ", (void*)&arrayD[0].double_field - (void*)&arrayD[0],
	 "int_field at ",    (void*)&arrayD[0].int_field    - (void*)&arrayD[0],
	 "char_field at ",   (void*)&arrayD[0].char_field   - (void*)&arrayD[0],
	 "char_field2 at ",  (void*)&arrayD[0].char_field2  - (void*)&arrayD[0]);



  
  return;
}
