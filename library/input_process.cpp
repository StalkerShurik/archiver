#include "input_process.h"
#include "iostream"

Result InputChecker::Check(int argc, char **argv) {
    argc--;
    if (argc == 0) {
        std::cerr << "Incorrect input, use `archiver -h`" << std::endl;
        throw;
    }
    if (!strcmp(argv[1], "-h")) {
        if (argc == 1) {
            std::cout << "Use 'archiver -c archive_name file1 [file2 ...]' to compress file1 [file2 ...] to archive_name" << std::endl <<
                      "Use 'archiver -d archive_name' to decompress archive_name to file1 [file2 ...]" << std::endl;
            return Result::HELP;
        } else {
            std::cerr << "Incorrect input, did you mean `archiver -h`" << std::endl;
            throw;
        }
    }
    if (!strcmp(argv[1], "-d")) {
        if (argc > 2) {
            std::cerr << "Too many arguments, use `archiver -h`" << std::endl;
            throw;
        }
        return Result::DECOMPRESS;
    }
    if (!strcmp(argv[1], "-c") && argc >= 3) {
        for (size_t i = 3; i < argc + 1; ++i) {
            if (!strcmp(argv[i], argv[2])) {
                std::cerr << "the name of input and output is equal" << std::endl;
                throw;
            }
        }
        return Result::COMPRESS;
    }
    std::cerr << "Incorrect input, use `archiver -h`" << std::endl;
    throw;
}