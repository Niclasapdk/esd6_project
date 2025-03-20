#include <stdio.h>

#include <ezdsp5535_i2s.h>
#include "codec.h"

int main() {
	Int16 fuck;
    EZDSP5535_init( );
	printf("hello motherfucker");
	initCODEC();
	
	while (1) {
		EZDSP5535_I2S_readLeft(&fuck);
		EZDSP5535_I2S_writeLeft(fuck);
	}
}
