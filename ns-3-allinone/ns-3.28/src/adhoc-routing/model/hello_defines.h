#ifndef HELLO_DEFINES_H
#define HELLO_DEFINES_H

#include "defines.h"


#define HELLO_DEBUG             DEBUG && 1
#define HELLO_INTERVAL_MS       500     // How often to broadcast hello messages
#define HELLO_NEIGHBOR_TTL_MS   1000    // How long a neighbor relationship lasts

#if HELLO_INTERVAL_MS >= HELLO_NEIGHBOR_TTL_MS
    #error "HELLO_INTERVAL_MS should be << HELLO_NEIGHBOR_TTL_MS"
#endif

#endif