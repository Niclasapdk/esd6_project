#include <EZDSP5535.h>
#include "wait.h"

// will wait a bit longer but only used for configuration so not a problem
void wait_usec(Uint32 usec) {
	// Clock rate is 100 MHz instead of 12 MHz (8.333 approx 9)
	EZDSP5535_waitusec(usec*20);
}
