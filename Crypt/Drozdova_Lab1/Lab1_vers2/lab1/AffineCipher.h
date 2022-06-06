#pragma once

#include <numeric>
#include "Checkers.h"


int GetAMinus(int a, int n) {
	int st = EulerFunc(n) - 1;
	if (st < 0) return -1;
	return binpow(a, st, n);
}
bool CheckAffineKey(const std::string& key, const std::string& alphabet, int& iA, int& iB, int& iA_minus) {
	iA = 0, iB = 0;
	int n = alphabet.size();
	for (auto c : key) {
		if (c == ' ') {
			std::swap(iA, iB);
			continue;
		}
		if (c > '9' || c < '1') return  false;
		iA *= 10;
		iA += (c - '0');
	}
	std::swap(iA, iB);
	iA_minus = GetAMinus(iA,n);
	return (iA_minus > 0 && gcd(iA, n) == 1);
}
void EncryptTextAffine(std::string& text, int iA, int iB, std::string& alphabet, std::string& sEncryptedText) {
	int n = alphabet.size();
	for (int i = 0; i < text.size(); i++) {
		char c = text[i];
		if (c == '\n') {
			sEncryptedText += "\n";
		}
		else {
			int id = (alphabetTable[text[i]] * iA + iB) % n;
			sEncryptedText += alphabet[id];
		}
	}
}

void DecryptTextAffine(std::string& text, int iA_minus, int iB, std::string& alphabet, std::string& sDecryptedText) {
	int n = alphabet.size();
	for (auto c : text) {
		if (c == '\n') {
			sDecryptedText += "\n";
		}
		else {
			int id = iA_minus * (alphabetTable[c] - iB + n) % n;
			sDecryptedText += alphabet[id];
		}
	}
}