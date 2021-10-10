#include "library/compressor.h"
#include "library/input_process.h"
#include "library/decompressor.h"
#include "string"


int main(int argc, char* argv[]) {
    InputChecker checker;
    Result checker_result = checker.Check(argc, argv);
    if (checker_result == Result::HELP) {
        return 0;
    } else if (checker_result == Result::COMPRESS) {
        std::vector<std::string> file_names;
        std::string out_file = argv[2];
        for (size_t i = 3; i < argc; ++i) {
            file_names.push_back(argv[i]);
        }
        Compressor compressor;
        compressor.Compress(out_file, file_names);
    } else {
        Decompressor decompressor;
        std::string file_name = argv[2];
        decompressor.Decompress(file_name);
    }
}
