import sys
import os
import random

alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwqyz"

DICTIONARY_WITH_HASHES = {}

colisions = 0

while colisions == 0:
        gen_str = alphabet[random.randint(0, len(alphabet) - 1)] + alphabet[random.randint(0, len(alphabet) - 1)]
        os.system(f"echo {gen_str} | python3.9 sponge.cpython-39.pyc >> in.txt")
        with open("in.txt", "r+") as inp:
            gen_hash = inp.readline().rstrip()
        if gen_hash in DICTIONARY_WITH_HASHES and DICTIONARY_WITH_HASHES[gen_hash] != gen_str:
            print(f"Find colision with values ({DICTIONARY_WITH_HASHES[gen_hash], gen_str})")
            colisions += 1
            break
        else:
            DICTIONARY_WITH_HASHES[gen_hash] = gen_str