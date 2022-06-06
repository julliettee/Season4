#pragma once

#include "Checkers.h"
int a11, a12, a21, a22; 
int B11, B12, B21, B22;

bool CheckHillKey(const std::string& key, const std::string& alphabet) {
    int b[4];
    int i = 0;
    for (int i = 0; i < 4; i++) {
        b[i] = -1;
    }
    for (auto c : key) {
        if (c == ' ') {
            i++;
            continue;
        }
        if (c > '9' || c < '0') return  false;
        if (i >= 4) return false;
        if (b[i] < 0) {
            b[i] = 0;
        }
        b[i] *= 10;
        b[i] += (c - '0');
    }
    for (int i = 0; i < 4; i++) {
        if (b[i] < 0 || b[i] >= alphabet.size()) return false;
    }
    B11 = b[0], B12 = b[1], B21 = b[2], B22 = b[3];
    int m = alphabet.size();
    int det = ((B11 * B22) % m - (B12 * B21) % m + m) % m;
    if ((det) % m == 0 || gcd((det), m) != 1) return false;
    a11 =(m + B22) % m, a12 = (m - B12) % m, a21 = (m - B21) % m, a22 = (m + B11) % m;
    int st = EulerFunc(m) - 1;
    if (st < 0) return false;
    det = binpow(det, st, m);
    a11 *= det;
    a12 *= det;
    a21 *= det;
    a22 *= det;

    a11 %= m;
    a12 %= m;
    a21 %= m;
    a22 %= m;
    return true;
}


void EncryptTextHill(std::string& text, std::string& alphabet, std::string& sEncryptedText) {
    if (text.size() % 2 == 1) {
        text += text[0];
    }
    int m = alphabet.size();
    for (int l = 0; l < text.size(); l += 2) {
        int b1 = alphabetTable[text[l]], b2 = alphabetTable[text[l + 1]];
        int c1 = (B11 * b1 + B12 * b2) % m, c2 = (B21 * b1 + B22 * b2) % m;
        sEncryptedText += alphabet[c1];
        sEncryptedText += alphabet[c2];
    }
}
void DecryptTextHill(std::string& text, std::string& alphabet, std::string& sDecryptedText) {
    if (text.size() % 2) {
        sDecryptedText = "Incorrect text.\n";
        return;
    }
    int m = alphabet.size();
    for (int l = 0; l < text.size(); l += 2) {
        int b1 = alphabetTable[text[l]], b2 = alphabetTable[text[l + 1]];
        int c1 = (a11 * b1 + a12 * b2) % m, c2 = (a21 * b1 + a22 * b2) % m;
        sDecryptedText += alphabet[c1];
        sDecryptedText += alphabet[c2];
    }
}