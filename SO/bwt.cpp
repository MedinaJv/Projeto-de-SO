#include "bwt.h"
#include <algorithm>

string bwt(const string& input) {
    if (input.empty()) return "";

    vector<string> rotations;
    
    string markedInput = input + "$"; // Caracter que marca o final da string
    int n = markedInput.size();

    for(int i = 0; i < n; i++)
        rotations.push_back(markedInput.substr(i) + markedInput.substr(0, i));

    sort(rotations.begin(), rotations.end());

    string transformed;

    for(const auto& rotation : rotations){
        transformed += rotation.back();
    }

    return transformed;
}


string inverseBwt(const string& bwt){
    if (bwt.empty()) return "";
    
    int n = bwt.size();
    vector<string> table(n, "");
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            table[j] = bwt[j] + table[j];
        }
        sort(table.begin(), table.end());
    }

    for(const auto& row : table){
        if(!row.empty() && row.back() == '$'){
            return row.substr(0, row.size() - 1);
        }
    }
  
    return "";
}