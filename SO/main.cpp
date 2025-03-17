#include "huffman.h"
#include "bwt.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
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
    }
    
    
    return 0;
}