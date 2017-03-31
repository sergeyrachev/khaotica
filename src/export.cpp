#include "export.h"

extern "C" double _nextbyte(double X) {
    return 123;
}

extern "C" double nextbyte(double X) {
    return 432;
}