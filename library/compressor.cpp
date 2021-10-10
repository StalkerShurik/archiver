#include "compressor.h"
#include <iostream>
void Compressor::Compress(std::string& compressed_file_name, std::vector<std::string>& file_names) {
    file_out.open(compressed_file_name);
    for (std::string& file_name : file_names) {
        ClearEverything();
        CountWords(file_name);
        symbols_count_ = 0;
        frequency_[256] = 1;
        frequency_[257] = 1;
        frequency_[258] = 1;
        symbols_count_ = frequency_.size();
        for (auto &symbol: frequency_) {
            std::shared_ptr<Node> node(new Node);
            node->is_leaf = true;
            node->symbol = symbol.first;
            sorted_.Insert(std::make_pair(symbol.second, node));
        }
        BuildBor();

        GetCodes("", root_);

        std::vector<std::pair<std::string, int>> code_symbol;

        for (auto &code: codes_) {
            code_symbol.emplace_back(std::make_pair(code.second, code.first));
        }
        std::sort(code_symbol.begin(), code_symbol.end(), comp);

        MakeCanonicalCodes(code_symbol);

        file_name_end_ = canonical_codes_[256];
        one_more_file_ = canonical_codes_[257];
        archive_end_ = canonical_codes_[258];

        IntToBin(symbols_count_);
        max_symbol_code_size_ = 1;

        for (auto &code: code_symbol) {
            max_symbol_code_size_ = std::max(max_symbol_code_size_, code.first.size());
            IntToBin(code.second);
            count_length_of_codes_[code.first.size()]++;
        }
        for (size_t length = 1; length <= max_symbol_code_size_; ++length) {
            IntToBin(static_cast<int>(count_length_of_codes_[length]));
        }

        CompressFile(file_name);

        if (file_name != file_names[file_names.size() - 1]) {
            StrToFile(one_more_file_);
        }
    }
    StrToFile(archive_end_);

    while (buffer_.size() < 8 && !buffer_.empty()) {
        buffer_ += "0";
    }
    FromBufferToFile();
}

Compressor::~Compressor() {
    ClearEverything();
}

void Compressor::ClearEverything() {
    symbols_count_ = 0;
    max_symbol_code_size_ = 1;
    sorted_.Clear();
    count_length_of_codes_.clear();
    canonical_codes_.clear();
    codes_.clear();
    root_ = nullptr;
    frequency_.clear();
}

void Compressor::BuildBor() {
    while (sorted_.Size() >= 2) {
        std::pair<size_t, std::shared_ptr<Node>> top1 = sorted_.GetMinAndDelete();
        std::pair<size_t, std::shared_ptr<Node>> top2 = sorted_.GetMinAndDelete();
        std::shared_ptr<Node> merge_node(new Node);
        merge_node->left = top1.second;
        merge_node->right = top2.second;
        sorted_.Insert(std::make_pair(top1.first + top2.first, merge_node));
    }
    root_ = sorted_.GetMin().second;
}

void Compressor::GetCodes(std::string current_code, std::shared_ptr<Node> current_node) {
    if (current_node->is_leaf) {
        codes_[current_node->symbol] = current_code;
    } else {
        GetCodes(current_code + "0", current_node->left);
        GetCodes(current_code + "1", current_node->right);
    }
}

bool Compressor::comp(std::pair<std::string, int> &p1, std::pair<std::string, int> &p2) {
    if (p1.first.size() == p2.first.size()) {
        return p1.second < p2.second;
    } else {
        return p1.first.size() < p2.first.size();
    }
}

void Compressor::MakeCanonicalCodes(std::vector<std::pair<std::string, int>> &code_symbol) {
    std::string code;
    for (size_t i = 0; i < code_symbol[0].first.size(); ++i) {
        code += "0";
    }

    canonical_codes_[code_symbol[0].second] = code;

    for (size_t i = 1; i < code_symbol.size(); ++i) {
        Add1(code);
        while (code.size() < code_symbol[i].first.size()) {
            code += "0";
        }
        canonical_codes_[code_symbol[i].second] = code;
    }
}

void Compressor::Add1(std::string &code) {
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

void Compressor::StrToFile(std::string &number) {
    buffer_ += number;
    FromBufferToFile();
}

void Compressor::IntToBin(size_t number) {
    std::string binary;
    while(number > 0) {
        binary += static_cast<char> (number % 2 + 48);
        number /= 2;
    }
    while (binary.size() < 9) {
        binary = binary + "0";
    }
    std::reverse(binary.begin(), binary.end());
    StrToFile(binary);
}

void Compressor::CountWords(std::string &file_name) {
    for (unsigned char symbol : file_name) {
        frequency_[symbol]++;
    }
    std::ifstream fin(file_name);
    if (!fin.is_open()) {
        std::cerr << "file " << file_name << " is not opened " << std::endl;
        throw;
    }
    unsigned char byte;
    fin.clear(); fin.seekg(0);
    while (1) {
        byte = fin.get();
        if (!fin.gcount()) {
            break;
        }
        frequency_[static_cast<int>(byte)]++;
    }
    fin.seekg(0); fin.clear();
}

void Compressor::FromBufferToFile() {
    while (buffer_.size() >= 8) {
        size_t code = 0;
        std::string tmp;
        for (size_t i = 0; i < 8; ++i) {
            code *= 2;
            code += static_cast<size_t> (buffer_[i]) - 48;
            tmp+=buffer_[i];
        }
        buffer_.erase(0, 8);
        file_out.put(static_cast<unsigned char>(code));
    }
}

void Compressor::CompressFile(std::string &file_name) {
    for (unsigned char symbol : file_name) {
        StrToFile(canonical_codes_[symbol]);
    }
    StrToFile(file_name_end_);

    std::ifstream fin(file_name);
    if (!fin.is_open()) {
        std::cerr << "file " << file_name << " is not opened" << std::endl;
        throw;
    }
    unsigned char byte;

    fin.clear(); fin.seekg(0);

    while (1) {
        byte = fin.get();
        if (!fin.gcount()) {
            break;
        }
        int code = static_cast<int> (byte);
        StrToFile(canonical_codes_[code]);
    }
}