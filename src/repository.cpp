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

repository::repository(const std::string &filename) : _bistream(filename.c_str()){

}

double repository::readdouble() {
    double d;
    _bistream >> d;
    return d;
}

double repository::readint() {
    int d;
    _bistream >> d;
    return d;
}

double repository::readchar() {
    char d;
    _bistream >> d;
    return 0;
}

repository::~repository() {
    if(!_bistream.eof()){
        std::cerr << "Input was read incompletely" << std::endl;
    }
}
