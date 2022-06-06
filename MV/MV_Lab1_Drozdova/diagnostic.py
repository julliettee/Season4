import random
import math
import time


# поиск решения методом Гаусса (по столбцу)
def FindSolutionGauss(A, d):
  n = len(A)
  G = [[0] * n for i in range(n)]
  b = [0] * n
  for i in range(n):
    b[i] = d[i]
    for j in range(n):
      G[i][j] = A[i][j]
        
  for i in range(n):
    max_element = abs(G[i][i])
    index = i
    for j in range(i + 1, n):
      if (abs(G[j][i]) > max_element):
        max_element, index = abs(G[j][i]), j
    if (max_element == 0):
      break
    for j in range(n):
      G[i][j], G[index][j] = G[index][j], G[i][j]
    b[i], b[index] = b[index], b[i]
    for j in range(i + 1, n):
      div = G[j][i] / G[i][i] 
      b[j] = b[j] - b[i] * div
      for l in range(i, n):
        G[j][l] -= div * G[i][l]
  i = n - 1
  result = [0] * n
  while(i >= 0):
    left = 0
    for j in range(i + 1, n):
      left += result[j] * G[i][j]
    result[i] = (b[i] - left) / G[i][i]
    i-=1
# поиск нормы матрицы
def GetA_1(b):
  n = len(b)
  a = [[0] * (2 * n) for i in range(n)]
  for i in range(n):
    for j in range(n):
      a[i][j] = b[i][j]
    a[i][n + i] = 1
  # приведение левой части к верхнетреугольному виду
  for i in range(n - 1):
    for j in range(i+1, n):
      if (a[i][i] != 0):
        div = -a[i][j]/a[i][i]
        for l in range(i, 2 * n):
          a[j][l] += div * a[i][l]
  # приведение левой части к диагоальному виду
 
  for i in range(n - 1, -1, -1):
      for l in range (i - 1, -1, -1):
        div = - a[l][i]/a[i][i]
        a[l][i] = 0
        for j in range (n, 2* n):
          a[l][j] += a[i][j] * div
  # приведение левой части к единичной матрице, копирование в b правой части,
  # которая будет обратной матрицей к заданной
  b = [[0] * n for i in range(n)]
  for i in range(n):
    div = a[i][i]
    for j in range(n, 2 * n):
      a[i][j] /= div
      if (j >= n):
        b[i][j - n] = a[i][j]
  return b
def GetNorm(a):
  n = len(a)
  norm = 0
  for i in range(n):
    cur = 0
    for j in range(n):
      cur += abs(a[i][j])
    norm = max(cur, norm)
  return norm
def GetVectorNorm(v):
  mx = 0
  n = len(v)
  for i in range(n):
    mx = max(mx, abs(v[i]))
  return mx
def Multiple(a, b, N, M, K):
  if (K > 1):
    c = [[0] * K for i in range(N)]
    for i in range (N):
      for j in range (K):
        c[i][j] = 0
        for l in range(M):
          c[i][j] += float(a[i][l] * b[l][j])
    return c
  else:
    c = [0] * N
    for i in range (N):
      for j in range (K):
        c[i] = 0
        x = 0
        for l in range(M):
          x = x + float(a[i][l] * b[l][j])
        c[i] = x
    return c
# поиск решения методом Гаусса (по столбцу)
def FindSolutionGauss(A, d):
    n = len(A)
    G = [[0] * n for i in range(n)]
    b = [0] * n
    for i in range(n):
        b[i] = d[i]
        for j in range(n):
            G[i][j] = A[i][j]

    for i in range(n):
        max_element = abs(G[i][i])
        index = i
        for j in range(i + 1, n):
            if (abs(G[j][i]) > max_element):
                max_element, index = abs(G[j][i]), j
        if (max_element == 0):
            break
        for j in range(n):
            G[i][j], G[index][j] = G[index][j], G[i][j]
        b[i], b[index] = b[index], b[i]
        for j in range(i + 1, n):
            div = G[j][i] / G[i][i]
            b[j] = b[j] - b[i] * div
            for l in range(i, n):
                G[j][l] -= div * G[i][l]

    i = n - 1
    result = [0] * n
    while (i >= 0):
        left = 0
        for j in range(n):
            left += result[j] * G[i][j]
        result[i] = (b[i] - left) / G[i][i]
        i -= 1
    return result
 
# метод релаксаци
def Relaxation(A, b, w, eps):
  N = len(A)
  x_i = [0] * N
  x_i1 = [0] * N
  r = [0] * N
  A1 = [[0] * N for i in range(N)]
  b1 = [0] * N
  go = 1
  c = 0
  val = []
  while(go):
    c+=1
    for i in range(N):
      before_i = 0
      after_i = 0
      for j in range(i):
        A1[i][j] = -A[i][j] * (w / A[i][i])
        before_i += A1[i][j] * x_i1[j]
      for j in range(i + 1, N):
        A1[i][j] = -A[i][j] * (w / A[i][i])
        after_i += A1[i][j] * x_i[j]
      b1[i] = b[i] * w / A[i][i]
      x_i1[i] = before_i + after_i + b1[i] - x_i[i] * (w - 1)
    for i in range(N):
      r[i] = x_i1[i] - x_i[i]
    for i in range(N):
      x_i[i] = x_i1[i]
    norm = GetVectorNorm(r)
    val.append(norm)
    if (norm < eps):
       break
  return x_i1, c, val 
N  = 256
V = 3
M = 3/4
Lborder = - (2**M)
Rborder = (2**M)
A2 = [[0] * N for i in range(N)]
f = open('MaxEAMatrix.txt')
l = 0
for line in f:
    spis = line.split()
    for j in range(N):
        A2[l][j] = float(spis[j])
    l+= 1
f.close()
x2 =  [[0] * 1 for i in range(N)]
for i in range(N):
  x2[i][0] = random.uniform(Lborder, Rborder)
b2 = Multiple(A2, x2, int(N), int(N), int(1))  
A_2 = GetA_1(A2)
norm = GetNorm(A2)
norm_1 = GetNorm(A_2)
delta3 = [0] * N
delta4 = [0] * N
delta5 = [0] * N
delta6 = [0] * N

b3 = [0] * N
b4 = [0] * N
b5 = [0] * N
b6 = [0] * N
for i in range(N):
    b3[i] = b2[i]
    b4[i] = b2[i]
    b5[i] = b2[i]
    b6[i] = b2[i]


b3[21] += 11 ** (-3)
b3[119] += 23 * 9 ** (-3)
b3[253] += 10 ** (-3)

b4[19] -= 21 * 35 ** (-4)
b4[249] -= 87 * 63 ** (-4)
b4[179] -= 31 * 9 ** (-4)

b5[137] += 21* 31 ** (-5)
b5[244] += 43* 18 ** (-5)
b5[235] += 5 * 43 ** (-5)

b6[217] -= 1 * 2 ** (-6)
b6[253] -= 17 * 2 ** (-6)
b6[254] -= 35 * 2 ** (-6)

resultGauss3 = FindSolutionGauss(A2, b3)
resultGauss4 = FindSolutionGauss(A2, b4)
resultGauss5 = FindSolutionGauss(A2, b5)
resultGauss6 = FindSolutionGauss(A2, b6)
for i in range(N):
    delta3[i] = x2[i][0] - resultGauss3[i]
    delta4[i] = x2[i][0] - resultGauss4[i]
    delta5[i] = x2[i][0] - resultGauss5[i]
    delta6[i] = x2[i][0] - resultGauss6[i]
norm = GetVectorNorm(x2[0])
print("Относительная погрешность 1: ", GetVectorNorm(delta3) / norm)
print("Относительная погрешность 2:", GetVectorNorm(delta4)/ norm)
print("Относительная погрешность 3:", GetVectorNorm(delta5)/ norm)
print("Относительная погрешность 4:", GetVectorNorm(delta6)/ norm)


import matplotlib.pyplot as plt
fig, (ax2) = plt.subplots(1)
ax2.plot(Relaxation(A2, b2, 0.8, eps)[2], label='w=0.8')
ax2.plot(Relaxation(A2, b2, 1.0, eps)[2], label='w=1.0')
ax2.plot(Relaxation(A2, b2, 1.2, eps)[2], label='w=1.2')
ax2.legend(loc='lower right')

ax2.set_yscale('log')
ax2.set_xlabel('k')
ax2.set_ylabel('Ax^k - b')
plt.subplots_adjust(left=0.1, bottom=0.1, right=0.9, top=0.9, wspace=0.4, hspace=0.4)
ax2.title.set_text('Max cond matrix A statistic')
fig = plt.gcf()
fig.set_size_inches(20, 7)
plt.show()