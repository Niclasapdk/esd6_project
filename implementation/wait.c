#include <EZDSP5535.h>
#include "wait.h"

// replaces EZDSP5535_waitusec
void wait_usec(Uint32 usec) {
    // wait longer to ensure setup is complete
    EZDSP5535_waitusec(usec*2);
}
