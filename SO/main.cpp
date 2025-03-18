#include "huffman.h"
#include "bwt.h"
#include "mtf.h"

int main(int argc, char* argv[]) {
    /*if (argc != 4) {
        cerr << "Uso: ./huffman [C/D] arquivo_entrada arquivo_saida" << endl;
        return 1;
    }
    
    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];
    
    if (mode == "C") {
        compressFile(inputFile, outputFile);
    } else if (mode == "D") {
        decompressFile(inputFile, outputFile);
    } else {
        cerr << "Modo inválido. Use 'C' para compressão e 'D' para descompressão." << endl;
    }*/

    string input = "eu estou tentando fazer isso funcionar agora com ãããããç letras estranhas do português";
    cout << "1_ Original: " << input << endl;

    string inputBwt = bwt(input);
    cout << "2_ BWT: " << inputBwt << endl;

    string inputMtf = mtf(inputBwt);
    cout << "3_ MTF: " << inputMtf << endl;

    string outputMtf = inverseMtf(inputMtf);
    cout << "4_ Inverse MTF: " << outputMtf << endl;

    string outputBwt = inverseBwt(outputMtf);
    cout << "5_ Inverse BWT: " << outputBwt << endl;


    
    return 0;
}