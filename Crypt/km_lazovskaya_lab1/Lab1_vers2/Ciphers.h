#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

std::unordered_map<char, int> alphabetTable;


const int MAX_A = 1e5;

// поиск НОД 2ух чисел
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
// функция Эйлера ищет количество чисел, меньших х и взаимопростых с х
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


inline bool ExistsFile(const std::string& name) {
    if (FILE* file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}
// проверка алфавита: все символы должны быть различные, должен быть пробел
bool CheckAlphabet(std::string& alphabet) {
    std::unordered_set <int> symbols;
    for (int i = 0; i < alphabet.size(); i++) {
        if (symbols.count(alphabet[i])) {
            return false;
        }
        alphabetTable[alphabet[i]] = i;
        symbols.insert(alphabet[i]);
    }
    
    return true;
}


// проверка текста: все символы должны быть из алфавита
bool CorrectInputText(std::string&str) {
    for (char c :str) {
        if (!alphabetTable.count(c)) {
            return false;
        }
    }
    return true;
}

// бинарное возведение в степень числа а в степень st по модулю М
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

int a11, a12, a21, a22;
int B11, B12, B21, B22;

/*
В качестве ключа задаются четыре символа из алфавита, (k11, k12, k21, k22),
записанных подряд (без разделителей). Из них и формируется ключевая
матрица 2∗2. При этом их числовое представление (k11 ∗k22−
k12 ∗ k21) mod M != 0, (k11 ∗ k22 − k12 ∗ k21, M) = 1 и k11, k12, k21, k22 ∈ ZM.
*/
bool CheckHillKey(const std::string& key, const std::string& alphabet) {
    int b[4];
    int i = 0;
    for (int i = 0; i < 4; i++) {
        b[i] = -1;
    }
    // парсинг строки на числа
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

    // проверка, чтобы все числа были меньше, чем размер алфавита
    for (int i = 0; i < 4; i++) {
        if (b[i] < 0 || b[i] >= alphabet.size()) return false;
    }
    B11 = b[0], B12 = b[1], B21 = b[2], B22 = b[3];
    int m = alphabet.size();
    // поиск определителя
    int det = ((B11 * B22) % m - (B12 * B21) % m + m) % m;
    // проверка условий для ключа
    //std::cout << det << m << gcd((det), m);
    if ((det) % m == 0 || gcd((det), m) != 1) return false;
    // поиск значений для обратной матрицы А (нужна для расшифровки)
    a11 = (m + B22) % m, a12 = (m - B12) % m, a21 = (m - B21) % m, a22 = (m + B11) % m;
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


void EncryptHill(std::string& str, std::string& alphabet, std::string& sOutput) {
    if (str.size() % 2 == 1) {
        str += str[0];
    }
    int m = alphabet.size();
    for (int l = 0; l < str.size(); l += 2) {
        int b1 = alphabetTable[str[l]], b2 = alphabetTable[str[l + 1]];
        // перемножение матрицы на вектор
        int c1 = (B11 * b1 + B12 * b2) % m, c2 = (B21 * b1 + B22 * b2) % m;
        sOutput += alphabet[c1];
        sOutput += alphabet[c2];
    }
}
void DecryptTextHill(std::string& str, std::string& alphabet, std::string& sOutput) {
    // зашифрованный текст должен быть четной длины
    if (str.size() % 2) {
        sOutput = "Некорректный текст.\n";
        return;
    }
    int m = alphabet.size();
    for (int l = 0; l < str.size(); l += 2) {
        int b1 = alphabetTable[str[l]], b2 = alphabetTable[str[l + 1]];
        // перемножение матрицы на вектор
        int c1 = (a11 * b1 + a12 * b2) % m, c2 = (a21 * b1 + a22 * b2) % m;
        sOutput += alphabet[c1];
        sOutput += alphabet[c2];
    }
}


/*
В качестве ключа задается символ из алфавита. При этом его числовое
представление k ∈ ZM.
*/


bool CheckShiftKey(const std::string& key, const std::string& alphabet, int& iKey) {
    iKey = 0;
    // парсинг строки в число
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
void DecryptShift(std::string& text, int iKey, std::string& alphabet, std::string& sOutput) {
    int n = alphabet.size();
    for (auto c : text) {
        int id = (alphabetTable[c] - iKey + n) % n;
        sOutput += alphabet[id];
    }
}

void EncryptShift(std::string& str, int iKey, std::string& alphabet, std::string& sOutput) {
    int n = alphabet.size();
    for (char c: str) {
        int id = (alphabetTable[c] + iKey) % n;
        sOutput += alphabet[id];
    }
}


std::unordered_map <char, char> encryptKeyTable, decryptKeyTable;
/*

В качестве ключа задается вторая строка подстановки. При этом она
должна содержать все символы алфавита, записанные в произвольном порядке по одному разу каждый, 
а числовое представление любого символа
ключа ki ∈ ZM

*/
bool CheckSSCKey(const std::string& key, const std::string& alphabet) {
    int n = alphabet.size(), m = key.size();
    // длина ключа должна совпадать с размером алфавита
    if (n != m) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        char cK = key[i], cA = alphabet[i];
        encryptKeyTable[cA] = cK;
        decryptKeyTable[cK] = cA;
    }
    // проверка на то, что каждый символ переопределен ровно 1 раз
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

void EncryptSSC(std::string& str, std::string& alphabet, std::string& sOutput) {
    for (auto c : str) {
         sOutput += encryptKeyTable[c];
    }
}
void DecryptSSC(std::string& str, std::string& alphabet, std::string& sOutput) {
    for (auto c : str) {
           sOutput += decryptKeyTable[c];
    }
}


std::vector < std::pair <char, int>> permutation;

/*
В качестве ключа задается строка, состоящая из попарно различных
символов алфавита, длиной не более M символов. Положение символов
ключа в алфавите определяет нижнюю строку подстановки. При этом числовое представление любого
символа ключа ki ∈ ZM. Если длина открытого текста не кратна длине ключа, то в его конец 
необходимо дописать один и тот же символ алфавита таким образом, чтобы длина получившегося
открытого текста была кратна длине ключа

*/

bool CheckTranspositionKey(const std::string& key, const std::string& alphabet) {
    int n = alphabet.size(), m = key.size();
    if (n < m) {
        return false;
    }
    std::unordered_set <char> symbols;
    for (char cA : alphabet) {
        symbols.insert(cA);
    }
    int i = 0;
    for (char c : key) {
        permutation.push_back({ c, i });
        i++;
        if (!symbols.count(c)) {
            return false;
        }
    }
    std::sort(permutation.begin(), permutation.end());
   
    return true;
}



void EncryptTransposition(std::string& str, std::string& key, std::string& sOutput) {
    int n = key.size();
    // длина строки должна быть кратна длине ключа
    while (int(str.size()) % n) {
        str += key[0];
    }
    int m = int(str.size()) / n;
    std::vector <std::string> table;
    // формирование таблицы
    for (int i = 0; i < m; i++) {
        std::string cur = "";
        for (int j = 0; j < n; j++) {
            cur += str[i * n + j];
        }
        table.push_back(cur);
    }

    // выводи части в соответсвии с ключом
    for (auto to : permutation) {
        int j = to.second;
        for (int i = 0; i < m; i++) {
            sOutput += table[i][j];
        }
    }
}


void DecryptTransposition(std::string& str, std::string& key, std::string& sOutput) {
    int n = key.size();
    // после зашифрования у строки длина должна быть кратной длине ключа
    if (int(str.size()) % n) {
        sOutput = "Некорректный текст.\n";
        return;
    }
    int m = int(str.size()) / n;
    std::vector <std::string> table;
    for (int i = 0; i < m; i++) {
        std::string cur;
        cur.resize(n);
        table.push_back(cur);
    }
    sOutput.resize(n * m, '\0');
    for (int i = 0; i < n * m; i++) {
        int j = i % m;
        int l = permutation[i / m].second + j * n;
        sOutput[l] = str[i];

    }
}


std::vector <std::string> table;
std::string myAlphabet;


/*

В качестве ключа задается строка произвольной длины, состоящая из
символов алфавита. При этом числовое представление любого символа
ключа ki ∈ ZM.

*/
bool CheckVigenereKey(const std::string& key, const std::string& alphabet) {
    int n = alphabet.size(), m = key.size();
    myAlphabet = alphabet;
    if (n < m) {
        return false;
    }
    std::unordered_set <char> symbols;
    for (char cA: alphabet) {
        symbols.insert(cA);
    }
    // символы ключа должны быть из алфавита
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


void EncryptVigenere(std::string& str, std::string& key, std::string& sOutput) {
    int n = str.size(), m = key.size();
    for (int i = 0; i < n; i++) {
        int l = alphabetTable[str[i]], r = alphabetTable[key[i % m]];
        sOutput += table[r][l];
    }
}
void DecryptVigenere(std::string& str, std::string& key, std::string& sOutput) {
    int n = str.size(), m = key.size();
    for (int i = 0; i < n; i++) {
        int r = alphabetTable[key[i % m]];
        for (int l = 0; l < table.size(); l++) {
            if (table[r][l] == str[i]) {
                sOutput += myAlphabet[l];
            }
        }
    }
}


int GetAMinus(int a, int n) {
    int st = EulerFunc(n) - 1;
    if (st < 0) return -1;
    return binpow(a, st, n);
}

/*
В качестве ключа задается пара символов из алфавита(k1, k2),
записсанных подряд(без разделителей).
При этом их числовое представление (k1, M) = 1 и k1, k2 ∈ ZM.
*/
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
    iA_minus = GetAMinus(iA, n);
    return (iA_minus > 0 && gcd(iA, n) == 1);
}

void EncryptAffine(std::string& str, int iA, int iB, std::string& alphabet, std::string& sOutput) {
    int n = alphabet.size();
    for (int i = 0; i < str.size(); i++) {
        char c = str[i];
        int id = (alphabetTable[str[i]] * iA + iB) % n;
        sOutput += alphabet[id];
    }
}

void DecryptAffine(std::string& str, int iA_minus, int iB, std::string& alphabet, std::string& sOutput) {
    int n = alphabet.size();
    for (auto c : str) {
        int id = iA_minus * (alphabetTable[c] - iB + n) % n;
        sOutput += alphabet[id];
    }
}