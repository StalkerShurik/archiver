#pragma once

#include "algorithm"
#include "fstream"
#include "iostream"
#include "memory"
#include "my_heap.h"
#include "queue"
#include "set"
#include "string"
#include "tuple"
#include "vector"
#include "unordered_map"

class Compressor {
public:
    void Compress(std::string& compressed_file_name, std::vector<std::string>& file_names);
    ~Compressor();

private:
    struct Node{
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        bool is_leaf;
        int symbol;
        Node() {
            left = nullptr;
            right = nullptr;
            is_leaf = false;
            symbol = -1;
        }
    };
    std::unordered_map<int, size_t> frequency_;
    std::unordered_map<int, std::string> codes_;
    std::unordered_map<int, std::string> canonical_codes_;
    std::unordered_map<size_t, size_t> count_length_of_codes_;
    MyHeap<std::pair<size_t, std::shared_ptr<Node>>> sorted_;
    std::shared_ptr<Node> root_;
    std::string file_name_end_;
    std::string one_more_file_;
    std::string archive_end_;
    std::string buffer_;
    std::ofstream file_out;
    size_t symbols_count_;
    size_t max_symbol_code_size_;
    void ClearEverything();

    void BuildBor();

    void GetCodes(std::string current_code, std::shared_ptr<Node> current_node);

    static bool comp(std::pair<std::string, int>& p1, std::pair<std::string, int>& p2);

    void MakeCanonicalCodes(std::vector<std::pair<std::string, int>>& code_symbol);

    void Add1(std::string& code);

    void StrToFile(std::string& number);

    void IntToBin(size_t number);

    void CountWords(std::string& file_name);

    void FromBufferToFile();

    void CompressFile(std::string& file_name);
};
