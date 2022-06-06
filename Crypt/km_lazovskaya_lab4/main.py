import sys
import os
import random

alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwqyz"

all_hashes = {}

colisions = 0

while colisions == 0:
        gen_str = alphabet[random.randint(0, len(alphabet) - 1)] + alphabet[random.randint(0, len(alphabet) - 1)]
        os.system(f"echo {gen_str} | python3.9 sponge.cpython-39.pyc >> in.txt")
        with open("in.txt", "r") as inp:
            gen_hash = inp.readline().rstrip()
        if gen_hash in all_hashes and all_hashes[gen_hash] != gen_str:
            print(f"Find colision with values ({all_hashes[gen_hash], gen_str})")
            colisions += 1
            break
        else:
            all_hashes[gen_hash] = gen_str