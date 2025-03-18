#include "bwt.h"
#include "sais.h"          // Inclui a nossa implementação SA-IS
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

/*
Implementação geral da transformação de Burrows-Wheeler, visando
a reordenação de dados de maneira a otimizar a compressão pelo
algoritmo de Huffman.
*/

string bwt(const string& input) {
    if (input.empty())
        return "";

    string markedInput = input + "\x03";
    int n = markedInput.size();

    vector<int> s(n);
    for (int i = 0; i < n; i++) {
        if (i == n - 1)
            s[i] = 0;
        else
            s[i] = static_cast<int>(markedInput[i]) + 1;
    }

    int K = *max_element(s.begin(), s.end()) + 1;

    //Implementação do SA_IS aqui
    vector<int> sa = SAIS::SA_IS(s, K);

    string transformed;
    transformed.resize(n);
    for (int i = 0; i < n; i++) {
        int idx = sa[i];
        transformed[i] = markedInput[(idx + n - 1) % n];
    }

    cout << "BWT Processado com SA-IS." << endl;
    return transformed;
}

string inverseBwt(const string &bwtStr) {
    if (bwtStr.empty())
        return "";

    size_t n = bwtStr.size();
    vector<int> freq(256, 0);
    for (char c : bwtStr)
        freq[static_cast<unsigned char>(c)]++;

    vector<int> starts(256, 0);
    int sum = 0;
    for (int c = 0; c < 256; c++) {
        if (freq[c] != 0) {
            starts[c] = sum;
            sum += freq[c];
        }
    }

    vector<int> occ(256, 0);
    vector<int> LF(n, 0);
    for (size_t i = 0; i < n; ++i) {
        unsigned char c = bwtStr[i];
        LF[i] = starts[c] + occ[c];
        occ[c]++;
    }

    int row = -1;
    for (size_t i = 0; i < n; i++) {
        if (bwtStr[i] == '\x03') {
            row = static_cast<int>(i);
            break;
        }
    }
    if (row == -1)
        return "";

    string original;
    original.resize(n - 1);
    for (int i = static_cast<int>(n) - 2; i >= 0; --i) {
        row = LF[row];
        original[i] = bwtStr[row];
    }

    cout << "BWT Inverso Processado." << endl;
    return original;
}
