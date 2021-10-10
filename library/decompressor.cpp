#include "decompressor.h"

void Decompressor::Decompress(std::string &file_name) {
    end_of_file = false;
    file_input.open(file_name);
    if (!file_input.is_open()) {
        std::cerr << "file " << file_name << " is not opened" << std::endl;
        throw;
    }
    while (!end_of_file) {
        codes.clear();
        DecompressFile();
    }
}

void Decompressor::DecompressFile() {
    size_t symbols_count = static_cast<int>(NineBitsRead());
    std::vector<int> order (symbols_count);
    for (size_t i = 0; i < symbols_count; ++i) {
        order[i] = NineBitsRead();
    }
    int position = 0;
    std::string current_code;
    size_t current_length = 1;
    while (position < symbols_count) {
        int count = NineBitsRead();
        if (count == 0) {
            current_length++;
            continue;
        }
        if (position == 0) {
            for (size_t j = 0; j < current_length; ++j) {
                current_code += "0";
            }
            codes[current_code] = order[0];
            position++;
            count--;
        }
        for (size_t j = 0; j < count; ++j) {
            Add1(current_code);
            while (current_code.size() < current_length) {
                current_code += "0";
            }
            codes[current_code] = order[position];
            position++;
        }
        current_length++;
    }
    std::string file_name;

    std::string code = ReadUntilGetCode();

    while (codes[code] != 256) {
        file_name += static_cast<unsigned char> (codes[code]);
        code = ReadUntilGetCode();
    }

    if (file_name.empty()) {
        std::cerr << "Incorrect archived file name" << std::endl;
        throw;
    }

    std::ofstream file_out(file_name);

    code = ReadUntilGetCode();
    while (codes[code] != 257 && codes[code] != 258) {
        unsigned char symbol = static_cast<unsigned char> (codes[code]);
        file_out.put(symbol);
        code = ReadUntilGetCode();
    }
    if (codes[code] == 258) {
        end_of_file = true;
    }
    file_out.close();
}

void Decompressor::Add1(std::string &code) {
    size_t size = code.size();
    size_t found = code.rfind('0');
    if (found == std::string::npos) {
        code = "1";
        for (size_t i = 0; i < size; ++i) {
            code += "0";
        }
    } else {
        code[found] = '1';
        for (size_t i = found + 1; i < code.size(); ++i) {
            code[i] = '0';
        }
    }
}

void Decompressor::ReadByte(size_t count) {
    unsigned char byte;
    for (size_t i = 0; i < count; ++i) {
        byte = file_input.get();
        if (!end_of_file && !file_input.gcount()) {
            std::cerr << "File is not correctly archived" << std::endl;
            throw;
        }
        std::string binary;
        int code = static_cast<int>(byte);
        while (code > 0) {
            binary += static_cast<char>(code % 2 + 48);
            code /= 2;
        }
        while (binary.size() < 8) binary += "0";
        std::reverse(binary.begin(), binary.end());
        buffer_ += binary;
    }
}

int Decompressor::NineBitsRead() {
    while (buffer_.size() < 9) {
        ReadByte(1);
    }
    int code = 0;
    for (size_t i = 0; i < 9; ++i) {
        code *= 2;
        code += static_cast<int>(buffer_[i] - 48);
    }
    buffer_.erase(0, 9);
    return code;
}

std::string Decompressor::ReadUntilGetCode() {
    std::string current_binary_code;
    while (codes.find(current_binary_code) == codes.end()) {
        if (!buffer_.empty()) {
            current_binary_code += buffer_[0];
            buffer_.erase(0, 1);
        } else {
            ReadByte(1);
        }
    }
    return current_binary_code;
}
