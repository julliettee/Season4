

import sys
import os 
import random

symbols = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwqyz"

def get_hash(string: str):
    #print("str ", string)
    os.system(f"echo {string} | python3.9 sponge.cpython-39.pyc >> temp.txt")
    with open("temp.txt", "r+") as inp:
        s = inp.readline().rstrip()
    #print("!!!! ", s)
    os.system("rm -rf temp.txt")
    return s


DICTIONARY_WITH_HASHES = {}

colisions = 0
counter = 0

while colisions < 50:
    while True:
        counter += 1
        gen_str = symbols[random.randint(0, len(symbols) - 1)] + \
                  symbols[random.randint(0, len(symbols) - 1)]

        gen_hash = get_hash(gen_str)
        #print("gen_hash ", gen_hash)
        if gen_hash in DICTIONARY_WITH_HASHES and DICTIONARY_WITH_HASHES[gen_hash] != gen_str:
            print(f"Warning: colision in values ({DICTIONARY_WITH_HASHES[gen_hash], gen_str})")
            colisions += 1
            break
        else:
            DICTIONARY_WITH_HASHES[gen_hash] = gen_str
print(f"Statistics: {colisions * 100 / counter}%")