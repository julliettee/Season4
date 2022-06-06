#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_SIZE 32
#define BUFF_SIZE 1024

using namespace std;

unsigned int holdrand = 0;

static void Srand(unsigned int seed) {
    holdrand = seed;
}

static int Rand(void) {
    return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
}

char* genere_key(void) {
    int i;
    static char key[KEY_SIZE + 1];
    const char charset[] =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "123456789";

    for (i = 0; i < KEY_SIZE; i++) {
        key[i] = charset[Rand() % (sizeof(charset) - 1)];
    }
    key[KEY_SIZE] = '\0';

    return key;
}

void crypt_buffer(unsigned char* buffer, size_t size, char* key) {
    size_t i;
    int j;

    j = 0;
    for (i = 0; i < size; i++) {
        if (j >= KEY_SIZE)
            j = 0;
        buffer[i] ^= key[j];
        j++;
    }
}

void crypt_file(FILE* in, FILE* out) {
    unsigned char buffer[BUFF_SIZE];
    char* key;
    size_t size;

    key = genere_key();

    printf("[+] Using key : %s\n", key);

    do {
        size = fread(buffer, 1, BUFF_SIZE, in);
        crypt_buffer(buffer, size, key);
        fwrite(buffer, 1, size, out);

    } while (size == BUFF_SIZE);
}
vector<int> encrypt_file(unsigned char str[BUFF_SIZE]) {
    vector<int> res;
    for (int day = 19052; day < 19083; day++) {
        int have = day * 24 * 60 * 60;
        for (int h = 0; h < 24; h++) {
            for (int m = 0; m < 60; m++) {
                for (int sec = 0; sec < 60; sec++) {
                    int H = have + h * 60 * 60 + m * 60 + sec;
                    Srand(H);
                    char* key = genere_key();
                    string msg2 = "";
                    for (int i = 0; i < BUFF_SIZE; i++) {
                        msg2 += char(str[i] ^ int(key[i % 32]));
                    }
                    if (msg2[0] == 'B' && msg2[1] == 'Z' && msg2[2] == 'h' && msg2[3] == '9') {
                        res.push_back(have + h * 60 * 60 + m * 60 + sec);
                    }
                }
            }
        }
    }
    return res;
}

int main(int argc, char** argv) {
    FILE* in, * out;

    if ((in = fopen("secret.bz2.crypt", "r")) == NULL) {
        perror("[-] fopen (in) ");
        return EXIT_FAILURE;
    }
    unsigned char buffer[10];
    size_t size;
    size = fread(buffer, 1, 10, in);
    vector<int> nums = encrypt_file(buffer);

    for (auto x : nums) {
        char path[128];
        Srand(x);
        snprintf(path, sizeof(path) - 1, "out%d.bz2", x);
        if ((out = fopen(path, "w+")) == NULL) {
            perror("[-] fopen (out) ");
            return EXIT_FAILURE;
        }
        fclose(in);
        in = fopen("secret.bz2.crypt", "r");
        crypt_file(in, out);
        cout << "Encrypted\n";
        fclose(out);
    }

    printf("[+] DONE.\n");
    return 0;
}

