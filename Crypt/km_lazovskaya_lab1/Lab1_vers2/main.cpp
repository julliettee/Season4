#include <iostream>
#include <fstream>
#include <numeric>

#include "Ciphers.h"



#define _CRT_SECURE_NO_WARNINGS

#define SHIFT 1
#define AFFINE 2
#define SIMPLE_SUBSTITUTION 3
#define HILL 4
#define TRANSPOSITION 5
#define VIGENERE 6






int main() {
    setlocale(LC_ALL, "Rus");
    std::system("chcp 1251");
    int xx = 42;
    while (xx % 22) {
        xx += 50;
    }
    std::cout << xx << "\n";
    std::string alphabet = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    if (!ExistsFile("alphabet.txt")) {
        std::ofstream fout("alphabet.txt");
        fout << alphabet;
        fout.close();
    }
    std::ifstream fin("alphabet.txt");
    std::getline(fin, alphabet);
    fin.close();
    // Проверка алфавита на повторяющиеся символы
    bool bCorrectAlphabet = CheckAlphabet(alphabet);
    if (!bCorrectAlphabet) {
        std::cout << "Ошибка: все символы должны быть различные. Дальнейшая работа невозможна.\n";
        return 0;
    }
    int m = alphabet.size();
    std::cout << "Выберите операцию: \n1 - зашифровать сообщение\n2 - расшифровать сообщение.\n";

    int choice;
    std::cin >> choice;
    if (choice != 1 && choice != 2) {
        std::cout << "Операция выбрана некорректно. Дальнейшая работа невозможна. \n";
        return 0;
    }

    std::cout << "Введите ваше сообщение. \n";
    std::string str;
    getline(std::cin,str);
    getline(std::cin,str);
    // проверка на то, что все символы текста входят в алфавит
    if (!CorrectInputText(str)) {
        std::cout << "Введеный текст некорректный. Дальнейшая работа невозможна. \n";
        return 0;
    }
    std::cout << "Выберите шифр: \n"
        "1. Шифр сдвига \n"
        "2. Афинный шифр \n"
        "3. Шифр простой подстановки \n"
        "4. Шифр Хилла \n"
        "5. Шифр перестановки \n"
        "6. Шифр Виженера \n"
        "\n";

    int choiceCipher;
    std::cin >> choiceCipher;
    if (choiceCipher < 1 || choiceCipher > 6) {
        std::cout << "Введеное значение некорректно. Дальнейшая работа невозможна. \n";
        return 0;
    }
    std::cout << "Введите значение ключа: \n";
    std::string keyString;
    getline(std::cin, keyString);
    getline(std::cin, keyString);
    switch (choiceCipher) {
    case SHIFT: {
        int iKey = 0;
        if (!CheckShiftKey(keyString, alphabet, iKey)) {
            std::cout << "Некорректное значение ключа. Дальнейшая работа невозможна.\n";
            return 0;
        }
        if (choice == 1) {
            std::string sOutput;
            EncryptShift(str, iKey, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        else {
            std::string sOutput;
            DecryptShift(str, iKey, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        break;
    }
    case AFFINE: {
        int iA = 0, iB = 0, iA_minus = 0;
        if (!CheckAffineKey(keyString, alphabet, iA, iB, iA_minus)) {
            std::cout << "Некорректное значение ключа. Дальнейшая работа невозможна.\n";
            return 0;
        }
        if (choice == 1) {
            std::string sOutput;
            EncryptAffine(str, iA, iB, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        else {
            std::string sOutput;
            DecryptAffine(str, iA_minus, iB, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
    }
    case SIMPLE_SUBSTITUTION: {
        if (!CheckSSCKey(keyString, alphabet)) {
            std::cout << "Некорректное значение ключа. Дальнейшая работа невозможна.\n";
            return 0;
        }
        if (choice == 1) {
            std::string sOutput;
            EncryptSSC(str, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        else {
            std::string sOutput;
            DecryptSSC(str, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
    }
    case TRANSPOSITION:{
        if (!CheckTranspositionKey(keyString, alphabet)) {
            std::cout << "Некорректное значение ключа. Дальнейшая работа невозможна.\n";
            return 0;
        }
        if (choice == 1) {
            std::string sOutput;
            EncryptTransposition(str, keyString, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        else {
            std::string sOutput;
            DecryptTransposition(str, keyString, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
    }
    case VIGENERE: {
        if (!CheckVigenereKey(keyString, alphabet)) {
            std::cout << "Некорректное значение ключа. Дальнейшая работа невозможна.\n";
            return 0;
        }
        if (choice == 1) {
            std::string sOutput;
            EncryptVigenere(str, keyString, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        else {
            std::string sOutput;
            DecryptVigenere(str, keyString, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
    }
    case HILL: {
        if (!CheckHillKey(keyString, alphabet)) {
            std::cout << "Некорректное значение ключа. Дальнейшая работа невозможна.\n";
            return 0;
        }
        if (choice == 1) {
            std::string sOutput;
            EncryptHill(str, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
        else {
            std::string sOutput;
            DecryptTextHill(str, alphabet, sOutput);
            std::cout << sOutput << "\n";
            return 0;
        }
    }

    }
    std::cout << alphabet;

    return 0;
}
