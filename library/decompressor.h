#pragma once
#include "iostream"
#include "fstream"
#include "string"
#include "unordered_map"
#include "vector"

class Decompressor {
public:
    void Decompress(std::string& file_name);
private:
    std::string buffer_;
    std::ifstream file_input;
    bool end_of_file;
    std::unordered_map<std::string, int> codes;

    void DecompressFile();

    void Add1(std::string& code);

    void ReadByte(size_t count);

    int NineBitsRead();

    std::string ReadUntilGetCode();
};