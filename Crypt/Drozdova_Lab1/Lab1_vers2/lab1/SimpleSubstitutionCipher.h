#pragma once

#include "Checkers.h"
#include <string>

std::unordered_map <char, char> encryptKeyTable, decryptKeyTable;

bool CheckSSCKey(const std::string& key, const std::string& alphabet) {
    int n = alphabet.size(), m = key.size();
    if (n != m) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        char cK = key[i], cA = alphabet[i];
        encryptKeyTable[cA] = cK;
        decryptKeyTable[cK] = cA;
    }
    for (char c : alphabet) {
        if (decryptKeyTable[encryptKeyTable[c]] != c) {
            return false;
        }
    }
    for (char c : key) {
        if (encryptKeyTable[decryptKeyTable[c]] != c) {
            return false;
        }
    }
    return true;
}

void EncryptTextSSC(std::string& text, std::string& alphabet, std::string& sEncryptedText) {
    int n = alphabet.size();
    for (auto c : text) {
        if (c == '\n') {
            sEncryptedText += "\n";
        }
        else {
            sEncryptedText += encryptKeyTable[c];
        }
    }
}
void DecryptTextSSC(std::string& text, std::string& alphabet, std::string& sDecryptedText) {
    int n = alphabet.size();
    for (auto c : text) {
        if (c == '\n') {
            sDecryptedText += "\n";
        }
        else {
            sDecryptedText += decryptKeyTable[c];
        }
    }
}