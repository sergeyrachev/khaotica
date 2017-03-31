//
// Created by sergey.rachev on 3/31/17.
//
#pragma once

#ifndef KHAOTICA_REPOSITORY_H
#define KHAOTICA_REPOSITORY_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <inttypes.h>

class repository {
public:
    repository(const std::string& filename);
    double read();
private:
    std::ifstream _bistream;
};

extern "C" double read_repository(uint64_t handle);
extern "C" repository* create_repository(const char* bitstream);
extern "C" void destroy_repository(repository*);

#endif //KHAOTICA_REPOSITORY_H
