#include "export.h"

extern "C" double _nextbyte(double X, int that) {
    return that - X;
}

extern "C" double nextbyte(double X, int that) {
    return that + X;
}