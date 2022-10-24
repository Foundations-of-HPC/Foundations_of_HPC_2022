#include <stdio.h>
#include <strings.h>

int main(int argc, char **argv)
{

	printf("\targc resides ar %p and its value is %d\n", &argc, argc);

	for( int ii = 0; ii < argc; ii++)
	{
		printf("pointer for argument %d is %p, argument %d is %s\n",
		ii, (argv+ii), ii, *(argv+ii) );
	}

	return 0;
}
