#include "huffman.h"
#include "bwt.h"
#include "mtf.h"

/*
Código de Huffman implementado através da abordagem de árvore de Huffman com
uso de fila de prioridade para a construção da árvore. O algoritmo é utilizado
para a compressão de dados após a transformação MTF e BWT.
*/

const string NULL_MARKER = "<<<NULO>>>";

HuffmanNode::HuffmanNode(char data, int freq){
    this->data = data;
    this->freq = freq;
    this->left = nullptr;
    this->right = nullptr;
}

bool Compare::operator()(HuffmanNode* a, HuffmanNode* b) {
    return a->freq > b->freq;
}

bool itsMarker(istream &in, const string &marker){
    streampos pos = in.tellg();
    string test;
    test.resize(marker.size());

    in.read(&test[0], marker.size());

    if(test == marker){
        return true;
    }
    in.seekg(pos);
    return false;
}

void serializeTree(HuffmanNode* root, ofstream& out) {
    if (!root) {
        out << NULL_MARKER; // Nó nulo
        return;
    }
    out.put(root->data);
    serializeTree(root->left, out);
    serializeTree(root->right, out);
}

HuffmanNode* deserializeTree(ifstream& in) {
    if(itsMarker(in, NULL_MARKER)){
        return nullptr;
    }
    char ch;
    in.get(ch);
    HuffmanNode* node = new HuffmanNode(ch, 0);
    node->left = deserializeTree(in);
    node->right = deserializeTree(in);
    return node;
}

void generateHuffmanCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root)
        return;
    
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = (code.empty() ? "0" : code);
        return;
    }
    
    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}


HuffmanNode* buildHuffmanTree(unordered_map<char, int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    
    for (auto pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }
    
    return pq.top();
}

void compressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    string inputText((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    cout << "COMPACTANDO: " << endl;
    //cout << "Original: " << inputText << endl;
    string bwtText = bwt(inputText);
    //cout << "BWT: " << bwtText << endl;
    string mtfText = mtf(bwtText);
    //cout << "MTF: " << mtfText << endl;
    
    unordered_map<char, int> freqMap;

    for (char ch : mtfText) {
        freqMap[ch]++;
    }
    
    HuffmanNode* root = buildHuffmanTree(freqMap);
    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);
    
    serializeTree(root, out); // Salvando a árvore no arquivo
    
    in.clear();
    in.seekg(0);
    string encodedText = "";
    for (char ch : mtfText) {
        encodedText += huffmanCodes[ch];
    }
    
    int padding = (8 - (encodedText.size() % 8)) % 8;
    out.put(padding);
    
    bitset<8> bits;
    for (size_t i = 0; i < encodedText.size(); i += 8) {
        string byteString = encodedText.substr(i, 8);
        while (byteString.size() < 8) byteString += '0';
        bits = bitset<8>(byteString);
        unsigned char byte = static_cast<unsigned char>(bits.to_ulong());
        out.write(reinterpret_cast<const char*>(&byte), sizeof(unsigned char));
    }

    cout << "Huffman Realizado" << endl;
    
    in.close();
    out.close();
}

void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);
    
    if (!in || !out) {
        cerr << "Erro ao abrir os arquivos." << endl;
        return;
    }
    
    HuffmanNode* root = deserializeTree(in); // Reconstruindo a árvore da compressão
    
    int padding = in.get();
    vector<unsigned char> byteData;
    char ch;
    while (in.get(ch)) {
        byteData.push_back(static_cast<unsigned char>(ch));
    }
    
    string encodedText = "";
    for (unsigned char byte : byteData) {
        bitset<8> bits(byte);
        encodedText += bits.to_string();
    }
    
    if (padding > 0) {
        encodedText = encodedText.substr(0, encodedText.size() - padding);
    }

    string decodedText = "";
    
    if(!root->left && !root->right){
        decodedText = string(encodedText.size(), root->data);
    }
    else{
        HuffmanNode* current = root;
        for (char bit : encodedText) {
            if (!current) {
                std::cerr << "Erro: Nó nulo encontrado durante a decodificação";
                break;
            }
        
            current = (bit == '0') ? current->left : current->right;
        
            if (!current) {
                std::cerr << "Erro: Caminho inválido na árvore para o bit " << bit << endl;
                break;
            }
        
            if (!current->left && !current->right) {
                decodedText += current->data;
                current = root;
            }
        }
    }

    cout << endl << "DESCOMPACTANDO: " << endl;
    //cout << "Huffman: " << decodedText << endl;
    string bwtText = inverseMtf(decodedText);
    //cout << "MTF: " << bwtText << endl;
    decodedText = inverseBwt(bwtText);    
    //cout << "BWT: " << decodedText << endl;
    out << decodedText;

    cout << "Huffman Reverso Realizado" << endl;
    
    in.close();
    out.close();
}