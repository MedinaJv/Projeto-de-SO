#include "bwt.h"
#include <algorithm>


string bwt(const string& input){
    if(input.empty())
        return "";

    string markedInput = input + "\x03";
    size_t n = markedInput.size();

    vector<size_t> indexes(n);
    for(size_t i = 0; i < n; ++i)
        indexes[i] = i;

    auto cmp = [&](size_t i, size_t j) -> bool {
        for(size_t k = 0; k < n; ++k){
            char a = markedInput[(i + k) % n];
            char b = markedInput[(j + k) % n];
            if(a != b)
                return a < b;
        }

        return false;
    };

    sort(indexes.begin(), indexes.end(), cmp);

    string transformed;
    transformed.resize(n);

    for(size_t i = 0; i < n; ++i){
        size_t idx = indexes[i];
        transformed[i] = markedInput[(idx + n - 1) % n];
    }
    cout << "BWT Processado" << endl;
    return transformed;
}

string inverseBwt(const string &bwtStr){
    if(bwtStr.empty())
        return "";

    size_t n = bwtStr.size();

    vector<int> freq(256, 0);
    for(char c : bwtStr)
        freq[static_cast<unsigned char>(c)]++;

    vector<int> starts(256, 0);
    int sum = 0;
    for(int c = 0; c < 256; c++){
        if(freq[c] != 0){
            starts[c] = sum;
            sum += freq[c];
        }
    }

    vector<int> occ(256, 0);
    vector<int> LF(n, 0);

    for(size_t i = 0; i < n; ++i){
        unsigned char c = bwtStr[i];
        LF[i] = starts[c] + occ[c];
        occ[c]++;
    }

    int row = -1;
    for(size_t i = 0; i < n; i++){
        if(bwtStr[i] == '\x03'){
            row = static_cast<int>(i);
            break;
        }
    }
    if(row == -1)
        return "";

    string original;
    original.resize(n - 1);

    for(int i = static_cast<int>(n) -2; i >= 0; --i){
        row = LF[row];
        original[i] = bwtStr[row];
    }
    
    cout << "BWT Reverso Processado" << endl;
    return original;
}