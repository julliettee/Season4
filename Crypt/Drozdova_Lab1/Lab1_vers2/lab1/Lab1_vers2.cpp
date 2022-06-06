#include <iostream>
#include <fstream>

#include "Checkers.h"
#include "ShiftCipher.h"
#include "AffineCipher.h"
#include "SimpleSubstitutionCipher.h"
#include "TranspositionCipher.h"
#include "VigenereCipher.h"
#include "HillCipher.h"


#define _CRT_SECURE_NO_WARNINGS

#define SHIFT 1
#define AFFINE 2
#define SIMPLE_SUBSTITUTION 3
#define HILL 4
#define TRANSPOSITION 5
#define VIGENERE 6


int main() {
    setlocale(LC_ALL, "Rus");
    std::string alphabet = "¿¡¬√ƒ≈®∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ ";
    if (!ExistsFile("alphabet.txt")) {
        std::ofstream fout("alphabet.txt");
        fout << alphabet;
        fout.close();
    }
    std::ifstream fin("alphabet.txt");
    std::getline(fin, alphabet);
    fin.close();
    bool bCorrectAlphabet = CheckAlphaBet(alphabet);
    if (!bCorrectAlphabet) {
        std::cout << "Error alphabet: all symbols should be different\n";
        return 0;
    }
    int m = alphabet.size();
    std::cout << "Print 'encrypt', if you want to encrypt your message. \n"
        "Print 'decrypt', if you want to decrypt your message. \n";
    std::string choice;
    std::cin >> choice;
    while (choice != "encrypt" && choice != "decrypt") {
        std::cout << "Incorrect format.\n Please, check your choice. \n";
        std::cin >> choice;
    }
    std::cout << "Enter file to " << choice << "\n";
    std::string fileToWork;
    std::cin >> fileToWork;
    while (!ExistsFile(fileToWork)) {
        std::cout << "This file doesn't exist. Try again.\n";
        std::cin >> fileToWork;
    }
    std::string textToWork;
    std::string sLine;
    fin.open(fileToWork);
    while (getline(fin, sLine)) {
        textToWork += sLine + "\n";
    }
    if (!textToWork.empty()) {
        textToWork.pop_back();
    }
    fin.close();
    if (!CorrectInputText(textToWork)) {
        std::cout << "Error: text to " << choice << " is not correct!\n";
        return 0;
    }
    std::cout << "Choose type of cipher: \n"
        "1. shift cipher \n"
        "2. affine cipher \n"
        "3. simple substitution cipher \n"
        "4. Hill cipher \n"
        "5. transposition cipher \n"
        "6. Vigenere cipher \n"
        "Enter a number of your choice. \n";
    std::string checkChoice;
    std::cin >> checkChoice;
    int choiceCipher = GetInt(checkChoice);
    while (choiceCipher < 1 || choiceCipher > 6) {
        std::cout << "Incorrect data, please, check your choice. \n";
        std::cin >> checkChoice;
        choiceCipher = GetInt(checkChoice);
    }
    std::cout << "Enter key's file name\n";
    std::string keyFile;
    std::cin >> keyFile;
    while (!ExistsFile(keyFile)) {
        std::cout << "This file doesn't exist. Try again.\n";
        std::cin >> keyFile;
    }
    fin.open(keyFile);
    std::string keyString;
    getline(fin, keyString);
    switch (choiceCipher) {
    case SHIFT: {
        int iKey = 0;
        if (!CheckShiftKey(keyString, alphabet, iKey)) {
            std::cout << "Error: Key is not correct\n";
            return 0;
        }
        if (choice == "encrypt") {
            std::string sEncryptedText;
            EncryptTextShift(textToWork, iKey, alphabet, sEncryptedText);
            std::cout << sEncryptedText << "\n";
            std::ofstream fout("shift/encrypted.txt");
            fout << sEncryptedText;
            fout.close();
            return 0;
        }
        else {
            std::string sDecryptedText;
            DecryptTextShift(textToWork, iKey, alphabet, sDecryptedText);
            std::cout << sDecryptedText << "\n";
            std::ofstream fout("shift/decrypted.txt");
            fout << sDecryptedText;
            fout.close();
            return 0;
        }
        break;
    }
    case AFFINE: {
        int iA = 0, iB = 0, iA_minus = 0;
        if (!CheckAffineKey(keyString, alphabet, iA, iB, iA_minus)) {
            std::cout << "Error: Key is not correct\n";
            return 0;
        }
        if (choice == "encrypt") {
            std::string sEncryptedText;
            EncryptTextAffine(textToWork, iA, iB, alphabet, sEncryptedText);
            std::cout << sEncryptedText << "\n";
            std::ofstream fout("affine/encrypted.txt");
            fout << sEncryptedText;
            fout.close();
            return 0;
        }
        else {
            std::string sDecryptedText;
            DecryptTextAffine(textToWork, iA_minus, iB, alphabet, sDecryptedText);
            std::cout << sDecryptedText << "\n";
            std::ofstream fout("affine/decrypted.txt");
            fout << sDecryptedText;
            fout.close();
            return 0;
        }
    }
    case SIMPLE_SUBSTITUTION: {
        if (!CheckSSCKey(keyString, alphabet)) {
            std::cout << "Error: Key is not correct\n";
            return 0;
        }
        if (choice == "encrypt") {
            std::string sEncryptedText;
            EncryptTextSSC(textToWork, alphabet, sEncryptedText);
            std::cout << sEncryptedText << "\n";
            std::ofstream fout("ssc/encrypted.txt");
            fout << sEncryptedText;
            fout.close();
            return 0;
        }
        else {
            std::string sDecryptedText;
            DecryptTextSSC(textToWork, alphabet, sDecryptedText);
            std::cout << sDecryptedText << "\n";
            std::ofstream fout("ssc/decrypted.txt");
            fout << sDecryptedText << "\n";
            fout.close();
            return 0;
        }
    }
    case TRANSPOSITION:{
        if (!CheckTranspositionKey(keyString, alphabet)) {
            std::cout << "Error: Key is not correct\n";
            return 0;
        }
        if (choice == "encrypt") {
            std::string sEncryptedText;
            EncryptTextTransposition(textToWork, keyString, sEncryptedText);
            std::cout << sEncryptedText << "\n";
            std::ofstream fout("transposition/encrypted.txt");
            fout << sEncryptedText;
            fout.close();
            return 0;
        }
        else {
            std::string sDecryptedText;
            DecryptTextTransposition(textToWork, keyString, sDecryptedText);
            std::cout << sDecryptedText << "\n";
            std::ofstream fout("transposition/decrypted.txt");
            fout << sDecryptedText << "\n";
            fout.close();
            return 0;
        }
    }
    case VIGENERE: {
        if (!CheckVigenereKey(keyString, alphabet)) {
            std::cout << "Error: Key is not correct\n";
            return 0;
        }
        if (choice == "encrypt") {
            std::string sEncryptedText;
            EncryptTextVigenere(textToWork, keyString, sEncryptedText);
            std::cout << sEncryptedText << "\n";
            std::ofstream fout("vigener/encrypted.txt");
            fout << sEncryptedText;
            fout.close();
            return 0;
        }
        else {
            std::string sDecryptedText;
            DecryptTextVigenere(textToWork, keyString, sDecryptedText);
            std::cout << sDecryptedText << "\n";
            std::ofstream fout("vigenere/decrypted.txt");
            fout << sDecryptedText << "\n";
            fout.close();
            return 0;
        }
    }
    case HILL: {
        if (!CheckHillKey(keyString, alphabet)) {
            std::cout << "Error: Key is not correct\n";
            return 0;
        }
        if (choice == "encrypt") {
            std::string sEncryptedText;
            EncryptTextHill(textToWork, alphabet, sEncryptedText);
            std::cout << sEncryptedText << "\n";
            std::ofstream fout("hill/encrypted.txt");
            fout << sEncryptedText;
            fout.close();
            return 0;
        }
        else {
            std::string sDecryptedText;
            DecryptTextHill(textToWork, alphabet, sDecryptedText);
            std::cout << sDecryptedText << "\n";
            std::ofstream fout("hill/decrypted.txt");
            fout << sDecryptedText << "\n";
            fout.close();
            return 0;
        }
    }

    }
    std::cout << alphabet;

    return 0;
}
