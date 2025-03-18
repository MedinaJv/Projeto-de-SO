#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <sstream>

using namespace std;

struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char data, int freq);
};

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b);
};

void generateHuffmanCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes);
HuffmanNode* buildHuffmanTree(unordered_map<char, int>& freqMap);
void compressFile(const string& inputFile, const string& outputFile);
void decompressFile(const string& inputFile, const string& outputFile);

#endif // HUFFMAN_H