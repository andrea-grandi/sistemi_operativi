#include "func.h"

unsigned int MCD(unsigned int a, unsigned int b){

	if (b > a) {
		unsigned int tmp = a;
		a = b;
		b = tmp;
	}

	while (b != 0){

		unsigned int r = a % b;
		a = b;
		b = r;

	}

	return a;
}

