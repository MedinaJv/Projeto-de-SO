#include "huffman.h"

HuffmanNode::HuffmanNode(char data, int freq){
    this->data = data;
    this->freq = freq;
    this->left = nullptr;
    this->right = nullptr;
};

bool Compare::operator()(HuffmanNode* a, HuffmanNode* b) {
    return a->freq > b->freq;
}

void serializeTree(HuffmanNode* root, ofstream& out) {
    if (!root) {
        out.put('#'); // Nó nulo
        return;
    }
    out.put(root->data);
    serializeTree(root->left, out);
    serializeTree(root->right, out);
}

HuffmanNode* deserializeTree(ifstream& in) {
    char ch;
    in.get(ch);
    if (ch == '#') return nullptr; // Nó nulo
    
    HuffmanNode* node = new HuffmanNode(ch, 0);
    node->left = deserializeTree(in);
    node->right = deserializeTree(in);
    return node;
}

void generateHuffmanCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root)
        return;
    
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
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
    
    unordered_map<char, int> freqMap;
    char ch;
    while (in.get(ch)) {
        freqMap[ch]++;
    }
    
    HuffmanNode* root = buildHuffmanTree(freqMap);
    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);
    
    serializeTree(root, out); // Salvando a árvore no arquivo
    
    in.clear();
    in.seekg(0);
    string encodedText = "";
    while (in.get(ch)) {
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
    HuffmanNode* current = root;
    
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
    for (char bit : encodedText) {
        current = (bit == '0') ? current->left : current->right;
        
        if (!current->left && !current->right) {
            decodedText += current->data;
            current = root;
        }
    }
    
    out << decodedText;
    
    in.close();
    out.close();
}