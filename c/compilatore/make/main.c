#include <stdlib.h>
#include "func.h"

int main(void){

	unsigned int a = 18;
	unsigned int b = 27;

	unsigned int mcd = MCD(a, b);

	if (mcd == 9)
		return EXIT_SUCCESS;
	else 
		return EXIT_FAILURE;
}

