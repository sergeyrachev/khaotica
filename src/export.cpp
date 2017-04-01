// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
