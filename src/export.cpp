#include "export.h"

#include "repository.h"

extern "C" double readdouble(double x, uint64_t that){
    repository *r = reinterpret_cast<repository *>(that);
    return r->readdouble();
}

extern "C" double readint(double x, uint64_t that){
    repository *r = reinterpret_cast<repository *>(that);
    return r->readint();
}

extern "C" double readchar(double x, uint64_t that){
    repository *r = reinterpret_cast<repository *>(that);
    return r->readchar();
}
