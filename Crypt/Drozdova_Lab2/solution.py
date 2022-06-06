import os, sys

result_file = ''
os.system("gcc crypt.c -o encode")
for day in range(19073 , 19074):
  have = day * 24 * 60 * 60
  hours = 9
  minutes = 36
  for sec in range(0, 60):
    H = have + hours * 60 * 60 + minutes * 60 + sec
    command = "./encode secret.bz2.crypt " + str(H)
    os.system(command)
    filename = "out" + str(H)
    err_code = os.system("bzip2 -d " + filename + ".bz2")
    if (err_code != 512):
        result_file += filename


print("your message was:")
os.system("cat " + result_file)
os.system('find . -type f -name "*.bz2" -delete')