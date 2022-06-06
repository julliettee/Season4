#pragma once
#include <string> 
#include "Checkers.h"

bool CheckShiftKey(const std::string& key, const std::string& alphabet, int& iKey) {
    iKey = 0;
    for (auto c : key) {
        if (c > '9' || c < '1') return  false;
        iKey *= 10;
        iKey += (c - '0');
    }
    if (iKey > alphabet.length()) {
        return false;
    }
    return true;
}
void DecryptTextShift(std::string& text, int iKey, std::string& alphabet, std::string& sDecryptedText) {
    int n = alphabet.size();
    for (auto c : text) {
        if (c == '\n') {
            sDecryptedText += "\n";
        }
        else {
            int id = (alphabetTable[c] - iKey + n) % n;
            sDecryptedText += alphabet[id];
        }
    }
}
void EncryptTextShift(std::string& text, int iKey, std::string& alphabet, std::string& sEncryptedText) {
    int n = alphabet.size();
    for (int i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == '\n') {
            sEncryptedText += "\n";
        }
        else {
            int id = (alphabetTable[text[i]] + iKey) % n;
            sEncryptedText += alphabet[id];
        }
    }
}