#include "export.h"

#include "repository.h"

extern "C" double _nextbyte(double X, uint64_t that) {
    return that - X;
}

extern "C" double nextbyte(double X, uint64_t that) {
    return X + read_repository(that);
}