#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

std::unordered_map<char, int> alphabetTable;


const int MAX_A = 1e5;

int gcd(int x, int y) {
    while (x && y) {
        if (x) {
            y %= x;
        }
        if (y) {
            x %= y;
        }
    }
    return x + y;
}
int EulerFunc(int x) {
    if (x > MAX_A) {
        return -1;
    }
    int ot = 0;
    for (int d = 1; d < x; d++) {
        if (gcd(d, x) == 1) {
            ot++;
        }
    }
    return ot;
}
int GetInt(std::string& s) {
    int x = 0;
    for (char c : s) {
        if (c > '9' || c < '0') return -1;
        x *= 10;
        x += (c - '0');
    }
    return x;
}

inline bool ExistsFile(const std::string& name) {
    if (FILE* file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}
bool CheckAlphaBet(std::string& alphabet) {
    std::unordered_set <int> symbols;
    bool bS = false;
    for (int i = 0; i < alphabet.size(); i++) {
        bS |= (alphabet[i] == ' ');
        if (symbols.count(alphabet[i])) {
            return false;
        }
        alphabetTable[alphabet[i]] = i;
        symbols.insert(alphabet[i]);
    }
    if (!bS) {
        alphabet += " ";
        std::cout << "WARNING: TEHERE ARE NO SPACES IN ALPHABET, SO IT WILL BE ADDED IN THE END OF THE ALPHABET\n";
    }
    return true;
}
bool CorrectInputText(std::string& textToWork) {
    bool sN = false;
    std::string textN;
    for (char c : textToWork) {
        if (c == '\n') {
            sN = true;
            textN += " ";
            continue;
        }
        textN += c;
        if (!alphabetTable.count(c)) {
            return false;
        }
    }
    textToWork = textN;
    if (sN) {
        std::cout << "WARNING: ALL ENTER SYMBOLS WILL BE REPLACED WITH SPACE\n";
    }
    return true;
}

int binpow(int a, int st, int m) {
    if (st == 1) return a;
    if (st == 0) return 1;
    if (st % 2 == 0) {
        long long b = binpow(a, st / 2, m);
        return (b * b) % m;
    }
    else {
        long long b = binpow(a, st - 1, m);
        b *= a;
        b %= m;
        return b;
    }
}

