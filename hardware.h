#ifndef HARDWARE

// Suport mbed hardware by including library
#ifdef __MBED__
#include "mbed.h"
#define HARDWARE "MBED"
#endif

#ifndef HARDWARE
#error "No hardware selected"
#endif

#endif
