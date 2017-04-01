// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
