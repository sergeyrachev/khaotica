#include "export.h"

extern "C" double _sinn(double X) {
    return 123;
}

extern "C" double sinn(double X) {
    return 432;
}