#pragma once

#include "Checkers.h"
#include <vector>
std::vector <std::string> table;
std::string myAlphabet;

bool CheckVigenereKey(const std::string& key, const std::string& alphabet) {
    int n = alphabet.size(), m = key.size();
    myAlphabet = alphabet;
    if (n < m) {
        return false;
    }
    std::unordered_set <char> symbols;
    for (int i = 0; i < n; i++) {
        char cA = alphabet[i];
        symbols.insert(cA);
    }
    for (char c : key) {
        if (!symbols.count(c)) {
            return false;
        }
    }
    table.push_back(alphabet);
    for (int i = 1; i < alphabet.size(); i++) {
        table.push_back("");
        int j = i;
        while (table.back().size() != alphabet.size()) {
            table.back() += alphabet[j];
            j++, j %= n;
        }
    }
    return true;
}


void EncryptTextVigenere(std::string& text, std::string& key, std::string& sEncryptedText) {
    int n = text.size(), m = key.size();
    for (int i = 0; i < n; i++) {
        int l = alphabetTable[text[i]], r = alphabetTable[key[i % m]];
        sEncryptedText += table[r][l];
    }
}
void DecryptTextVigenere(std::string& text, std::string& key, std::string& sDecryptedText) {
    int n = text.size(), m = key.size();
    for (int i = 0; i < n; i++) {
        int r = alphabetTable[key[i % m]];
        for (int l = 0; l < table.size(); l++) {
            if (table[r][l] == text[i]) {
                sDecryptedText += myAlphabet[l];
            }
        }        
    }
}


