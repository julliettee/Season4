#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "Checkers.h"


std::vector < std::pair <char, int>> permutation;
std::vector <int> inv;

bool CheckTranspositionKey(const std::string& key, const std::string& alphabet) {
    int n = alphabet.size(), m = key.size();
    if (n < m) {
        return false;
    }
    std::unordered_set <char> symbols;
    for (int i = 0; i < n; i++) {
        char cA = alphabet[i];
        symbols.insert(cA);
    }
    int i = 0;
    for (char c : key) {
        permutation.push_back({c, i});
        i++;
        if (!symbols.count(c)) {
            return false;
        }
    }
    std::sort(permutation.begin(), permutation.end());
    inv.resize(n, 0);
    for (int i = 0; i < m; i++) {
        inv[i] = permutation[i].second;
    }
    return true;
}



void EncryptTextTransposition(std::string& text, std::string& key, std::string& sEncryptedText) {
    int n = key.size();
    while (int(text.size()) % n) {
        text += key[0];
    }
    int m = int(text.size()) / n;
    std::vector <std::string> table;
    for (int i = 0; i < m; i++) {
        std::string cur = "";
        for (int j = 0; j < n; j++) {
            cur += text[i * n + j];
        }
        table.push_back(cur);
    }
    for (auto to : permutation) {
        int j = to.second;
        for (int i = 0; i < m; i++) {
            sEncryptedText += table[i][j];
        }
    }
}


void DecryptTextTransposition(std::string& text, std::string& key, std::string& sDecryptedText) {
    int n = key.size();
    while (int(text.size()) % n) {
        text += key[0];
    }
    int m = int(text.size()) / n;
    std::vector <std::string> table;
    for (int i = 0; i < m; i++) {
        std::string cur;
        cur.resize(n);
        table.push_back(cur);
    }
    sDecryptedText.resize(n * m, '\ ');
    for (int i = 0; i < n * m; i++) {
        int j = i % m;
        int l = permutation[i/m].second  + j * n;
        sDecryptedText[l] = text[i];
        
    }
}