#include "mtf.h"

#include <iostream>
#include <string>

/*
Implementação pós BWT para a transformação Move-to-Front. Visa transformar
a sequência gerada pelo BWT em uma lista de símbolos mais frequentes. Que
permite uma compressão mais eficaz pelo código de Huffman
*/


string mtf(const string &input){
    vector <unsigned char> symbols(256);
    iota(symbols.begin(), symbols.end(), 0);

    string output;
    output.reserve(input.size());

    for(unsigned char c : input){
        auto it = find(symbols.begin(), symbols.end(), c);
        int index = distance(symbols.begin(), it);
        output.push_back(static_cast<char>(index));

        symbols.erase(it);
        symbols.insert(symbols.begin(), c);
    }

    cout << "MTF Realizado" << endl;

    return output;
}


string inverseMtf(const string& encoded){
    vector <unsigned char> symbols(256);
    iota(symbols.begin(), symbols.end(), 0);

    string output;
    output.reserve(encoded.size());

    for(unsigned char index : encoded){
        int pos = static_cast<unsigned char>(index);
        unsigned char c = symbols[pos];
        output.push_back(c);

        symbols.erase(symbols.begin() + pos);
        symbols.insert(symbols.begin(), c);
    }

    cout << "MTF Reverso Realizado" << endl;

    return output;
}