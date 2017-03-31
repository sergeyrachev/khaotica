//
// Created by sergey.rachev on 3/31/17.
//

#include "repository.h"

extern "C" repository* create_repository(const char *bitstream){
    return new repository(bitstream);
}

void destroy_repository(repository * p) {
    delete p;
}

double read_repository(uint64_t handle) {
    return reinterpret_cast<repository*>(handle)->read();
}

repository::repository(const std::string &filename) : _bistream(filename.c_str()){

}

double repository::read() {
    double d;
    _bistream >> d;
    return d;
}
