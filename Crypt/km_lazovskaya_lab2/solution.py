import bz2


key = []
H = 0

def rand2():
  global H
  H = int(H * 214013 + 2531011)
  return (((H) >> 16) & 0x7fff)


def genKey():
  global key
  key = []
  str = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789'
  for i in range(32):
    key.append(str[rand2() % 61])


in_file = open("secret.bz2.crypt", "rb") 
data = in_file.read() 
in_file.close()
print((data))

print()

for day in range(19052 , 19083):
  have = day * 24 * 60 * 60
  for h in range(0, 24):
    for m in range(0, 60):
      for sec in range(0, 60):
        H = have + h * 60 * 60 + m * 60 + sec
        genKey()
        i = 0
        msg2 = ''
        for x in data:
            c = chr(x ^ ord(key[i % 32]))
            msg2 += chr(x ^ ord(key[i % 32]))
            i+=1
            if (i > 3):
                break
        if (msg2[0] == 'B' and msg2[1] == 'Z' and msg2[2] == 'h' and msg2[3] == '9'):
            print(have + h * 60 * 60 + m * 60 + sec)


