#ifndef ARCHIVER_INPUT_PROCESS_H
#define ARCHIVER_INPUT_PROCESS_H

#pragma once

#include "iostream"
#include "fstream"
#include "cstring"

enum class Result {
    HELP,
    COMPRESS,
    DECOMPRESS
};

class InputChecker {
public:
    Result Check(int argc, char* argv[]);
private:
};


#endif //ARCHIVER_INPUT_PROCESS_H
