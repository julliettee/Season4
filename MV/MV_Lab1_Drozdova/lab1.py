import random
import math
import time

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


# поиск обратной матрицы
def GetA_1(b):
    n = len(b)
    a = [[0] * (2 * n) for i in range(n)]
    for i in range(n):
        for j in range(n):
            a[i][j] = b[i][j]
        a[i][n + i] = 1
    # приведение левой части к верхнетреугольному виду
    for i in range(n - 1):
        for j in range(i + 1, n):
            if (a[i][j] != 0):
                div = -a[i][j] / a[i][i]
                for l in range(i, 2 * n):
                    a[j][l] += div * a[i][l]
    # приведение левой части к диагоальному виду

    for i in range(n - 1, -1, -1):
        for l in range(i - 1, -1, -1):
            div = - a[l][i] / a[i][i]
            a[l][i] = 0
            for j in range(n, 2 * n):
                a[l][j] += a[i][j] * div
    # приведение левой части к единичной матрице, копирование в b правой части,
    # которая будет обратной матрицей к заданной
    b = [[0] * n for i in range(n)]
    for i in range(n):
        div = a[i][i]
        for j in range(i, 2 * n):
            a[i][j] /= div
            if (j >= n):
                b[i][j - n] = a[i][j]
    return b

# поиск нормы матрицы
def GetNorm(a):
  n = len(a)
  norm = 0
  for i in range(n):
    cur = 0
    for j in range(n):
      cur += abs(a[i][j])
    norm = max(cur, norm)
  return norm



# поиск LUP разложения (по столбцу)
def GetLUP(A):
  n = len(A)
  U = [[0] * n for i in range(n)]
  for i in range(n):
    for j in range(n):
      U[i][j] = A[i][j]
  L = [[0] * n for i in range(n)]
  p_was = []
  p_now = []
  for i in range(n):
    L[i][i] = 1
  for i in range(n):
    max_element = abs(U[i][i])
    index = i
    for j in range(i + 1, n):
      if (abs(U[j][i]) > max_element):
        max_element, index = abs(U[j][i]), j
    if (max_element == 0):
      break
    p_was.append(i)
    p_now.append(index)
    for j in range(n):
      U[i][j], U[index][j] = U[index][j], U[i][j]
    for j in range(i + 1, n):
      L[j][i] = U[j][i] / U[i][i]
      for l in range(i, n):
        U[j][l] -= L[j][i] * U[i][l]
  p = []
  for i in range(n):
    p.append(i)
  for j in range(n):
      ii = int(p_was[j])
      jj = int(p_now[j])
      p[ii], p[jj] = p[jj], p[ii]

  for i in range(n):
    for j in range(i + 1, n):
      ii = int(p_was[j])
      jj = int(p_now[j])
      L[ii][i], L[jj][i] = L[jj][i], L[ii][i]
  return L, U, p


# поиск решения через LUP
def FindSolutionLUP(L, U, P, b):
  N = len(L)
  b1 = [0] * N
  for i in range(N):
    b1[i] = b[P[i]]
  solution = []
  solution.append(b1[0] / L[0][0])
  for i in range(1, N):
    left = 0
    for j in range(i):
      left += solution[j] * L[i][j]
    solution.append((b1[i] - left) / L[i][i])
  i = N - 1
  result = [0] * N
  while(i >= 0):
    left = 0
    for j in range(N):
      left += result[j] * U[i][j]
    result[i] = (solution[i] - left) / U[i][i]
    i-=1
  return result


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

def FindSolutionSQRT(A):
  N = len(A)
  SQRT = [[0] * N for i in range(N)]

  for i in range(N):
    for j in range(N):
      SQRT[i][j] = A[i][j]
  
  for i in range(N):
    for j in range(i+1, N):
      div = SQRT[j][i] / SQRT[i][i]
      for l in range(N):
        SQRT[j][l] -= div * SQRT[i][l]
    for j in range(i + 1, N):
      SQRT[i][j] /= SQRT[i][i]


  for i in range(N):
    for j in range(i+1, N):
      SQRT[i][j] *= abs(SQRT[i][i]) ** 0.5
      SQRT[i][j] *= abs(SQRT[i][i])/SQRT[i][i]
    SQRT[i][i] /= abs(SQRT[i][i]) ** 0.5

  y = []
  for i in range(0, N):
    left = 0
    for j in range(i):
      left += y[j] * SQRT[j][i]
    y.append((b[i] - left) / SQRT[i][i])

  for i in range(0, N):
    for j in range(i + 1, N):
      SQRT[i][j] *= abs(SQRT[i][i]) / SQRT[i][i]
    SQRT[i][i] *= abs(SQRT[i][i]) / SQRT[i][i]

  i = N - 1
  solution = [0] * N
  while(i >= 0):
    left = 0
    for j in range(i + 1, N):
      left += solution[j] * SQRT[i][j]
    solution[i] = (y[i] - left) / SQRT[i][i]
    i-=1
  return solution
def FindSolutionLDLT(L, D, LT, b):
  N = len(L)
  LD = Multiple(L, D, N, N, N)
  y = []
  y.append(b[0] / LD[0][0])
  for i in range(1, N):
    left = 0
    for j in range(i):
      left += y[j] * LD[i][j]
    y.append((b[i] - left) / LD[i][i])
  
  i = N - 1
  solution = [0] * N
  while(i >= 0):
    left = 0
    for j in range(N):
      left += solution[j] * LT[i][j]
    solution[i] = (y[i] - left) / LT[i][i]
    i-=1
  return solution
def FindLDLT(A):
  N = len(A)
  U = [[0] * N for i in range(N)]
  L = [[0] * N for i in range(N)]
  D = [[0] * N for i in range(N)]
  LT = [[0] * N for i in range(N)]
  for i in range(N):
    for j in range(N):
      U[i][j] = A[i][j]

  for i in range(N):
    for j in range(i + 1, N):
      L[j][i] = U[j][i] / U[i][i]
      LT[i][j] = L[j][i]
      for l in range(i, N):
        U[j][l] -= L[j][i] * U[i][l]

  for i in range(N):
    #L[i][i] = abs(A[i][j]) ** 0.5
    #LT[i][i] = abs(A[i][j]) ** 0.5
    #D[i][i] = A[i][j] / abs(A[i][j])
    D[i][i] = U[i][i]
    L[i][i] = 1
    LT[i][i] = 1
  return L, D, LT

def GetVectorNorm(v):
  mx = 0
  n = len(v)
  for i in range(n):
    mx = max(mx, abs(v[i]))
  return mx

def Relaxation(A, b, w, eps):
  N = len(A)
  x_i = [0] * N
  x_i1 = [0] * N
  r = [0] * N
  A1 = [[0] * N for i in range(N)]
  b1 = [0] * N
  go = 1
  c = 0
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
    if (norm < eps):
       break
  return x_i1, float(c)

N = 256
M = 3/4
val = (10**9)
Lborder = - (2**M)
Rborder = (2**M)
maxA = [[0] * N for i in range(N)]
maxNorm = 0
iter = 100
avg_inversive_time = 0
maxEA = 0
minEA = Rborder * 3 * N
avgEA = 0
avg_time_Gauss = 0
avg_time_buildLUP = 0
avg_time_solveLUP = 0
avg_time_solveSQRT = 0
avg_time_solveRelaxation = 0
avg_operRelax = 0
min_operRelax = val
max_operRelax = 0

min_normGauss = val
max_normGauss = 0
avg_normGauss = 0

min_normLUP = val
max_normLUP = 0
avg_normLUP = 0

min_normSQRT = val
max_normSQRT = 0
avg_normSQRT = 0

min_normRelax = val
max_normRelax = 0
avg_normRelax = 0

st = time.time()
while(1):
    #print("now ", iter, '{:.5f}'.format(time.time() - st))
    st = time.time()
    iter -= 1
    if (iter == 0):
        break
    A = [[0] * N for i in range(N)]
    for i in range(0, N):
        A[i][i] = 1
        for j in range(i):
            A[i][i] += abs(A[i][j])
        for j in range(i + 1, N):
            A[i][j] = random.uniform(Lborder, Rborder)
            A[j][i] = A[i][j]
            A[i][i] += abs(A[i][j])
    y = [[0] * 1 for i in range(N)]
    for i in range(N):
      y[i][0] = random.uniform(Lborder, Rborder)

    b = Multiple(A, y, int(N), int(N), int(1))

    start_time = time.time()
    A_1 = GetA_1(A)
    find_inverse_matrix_time = (time.time() - start_time)
    avg_inversive_time += find_inverse_matrix_time
    #print("Время нахождения обратной матрицы: ", find_inverse_matrix_time)

    norm = GetNorm(A)
    norm_1 = GetNorm(A_1)

    EA = norm * norm_1
    minEA = min(minEA, EA)
    if (maxEA < EA):
        for i in range(N):
            for j in range(N):
                maxA[i][j] = A[i][j]
    avgEA += EA
    minEA = min(EA, minEA)
    maxEA = max(EA, maxEA)
    #print("Число обусловленности:", EA)

    start_time = time.time()
    resultGauss = FindSolutionGauss(A, b)
    t = (time.time() - start_time)
    avg_time_Gauss += t
    #print("Время решения: ", t)
    #print("Решение, полученное методом Гаусса: ", resultGauss)
    delta_Gauss = [0] * N
    for i in range(N):
        delta_Gauss[i] = resultGauss[i] - y[i][0]
    normGauss = GetVectorNorm(delta_Gauss)

    min_normGauss = min(min_normGauss, normGauss)
    max_normGauss = max(max_normGauss, normGauss)
    avg_normGauss += normGauss




    start_time = time.time()
    L, U, P = GetLUP(A)
    t = (time.time() - start_time)
    avg_time_buildLUP += t
    #print("Время построения LUP: ", (time.time() - start_time))
    start_time = time.time()
    resultLUP = FindSolutionLUP(L, U, P, b)
    t = (time.time() - start_time)
    avg_time_solveLUP += t
    #print("Время решения: ", (time.time() - start_time))
    #print("Решение, полученное через LUP: ", resultLUP)

    delta_LUP = [0] * N
    for i in range(N):
        delta_LUP[i] = resultLUP[i] - y[i][0]
    normLUP = GetVectorNorm(delta_LUP)

    min_normLUP = min(min_normLUP, normLUP)
    max_normLUP = max(max_normLUP, normLUP)
    avg_normLUP += normLUP



    start_time = time.time()
    L, D, LT = FindLDLT(A)
    resultLDLT = FindSolutionSQRT(A)
    t = (time.time() - start_time)
    avg_time_solveSQRT += t
    #print("Время решения: ", (time.time() - start_time))
    #print("Решение, полученное методом квадратного корня: ", resultLDLT)

    delta_SQRT = [0] * N
    for i in range(N):
        delta_SQRT[i] = resultLDLT[i] - y[i][0]
    normSQRT = GetVectorNorm(delta_SQRT)

    min_normSQRT = min(min_normSQRT, normSQRT)
    max_normSQRT = max(max_normSQRT, normSQRT)
    avg_normSQRT += normSQRT


    w = 1 - 3 / 40
    eps = 0.00001
    start_time = time.time()
    solutionRelax, iter_amount = Relaxation(A, b, w, eps)

    t = (time.time() - start_time)
    avg_time_solveRelaxation += t
    # print("Время решения: ", t)
    # print("Решение, полученное методом релаксации: ", solutionRelax)

    avg_operRelax += iter_amount
    min_operRelax = min(iter_amount, min_operRelax)
    max_operRelax = max(iter_amount, max_operRelax)

    delta_Relax = [0] * N
    for i in range(N):
        delta_Relax[i] = solutionRelax[i] - y[i][0]
    normRelax = GetVectorNorm(delta_Relax)

    min_normRelax = min(min_normRelax, normRelax)
    max_normRelax = max(max_normRelax, normRelax)
    avg_normRelax += normRelax




print("Cреднее арифметическое число обусловленности:\t\t", '{:.15f}'.format(avgEA / 100))
print("Минимальное число обусловленности:\t\t\t", '{:.15f}'.format(minEA))
print("Максимальное число обусловленности:\t\t\t", '{:.15f}'.format(maxEA))
f = open('MaxEAMatrix.txt', 'w+')
for i in range(N):
    for j in range(N):
        f.write(str(maxA[i][j]))
        f.write(' ')
    f.write('\n')
f.close()
print("Среднее время нахождения обратной матрицы:\t\t", '{:.15f}'.format(avg_inversive_time/ 100))
print("Среднее время решения СЛАУ методом Гаусса:\t\t", '{:.15f}'.format(avg_time_Gauss/ 100))
print("Нормы разности решения методом Гаусса с точным решением:")
print("\t Средняя:\t", '{:.15f}'.format(avg_normGauss/ 100))
print("\t Минимальная:\t", '{:.15f}'.format(min_normGauss))
print("\t Максимальная:\t", '{:.15f}'.format(max_normGauss))
print("Среднее время построения LUP-разложения:\t\t", '{:.15f}'.format(avg_time_buildLUP/ 100))
print("Среднее время решения СЛАУ LUx = b1:\t\t\t", '{:.15f}'.format(avg_time_solveLUP/ 100))
print("Нормы разности решения с помощью LUP-разложения с точным решением:")
print("\t Средняя:\t", '{:.15f}'.format(avg_normLUP/ 100))
print("\t Минимальная:\t", '{:.15f}'.format(min_normLUP))
print("\t Максимальная:\t", '{:.15f}'.format(max_normLUP))
print("Среднее время решения СЛАУ методом квадратного корня:\t", '{:.15f}'.format(avg_time_solveSQRT/ 100))
print("Нормы разности решения методом квадратного корня с точным решением:")
print("\t Средняя:\t", '{:.15f}'.format(avg_normSQRT/ 100))
print("\t Минимальная:\t", '{:.15f}'.format(min_normSQRT))
print("\t Максимальная:\t", '{:.15f}'.format(max_normSQRT))
print("Среднее время решения СЛАУ методом релаксации:\t\t", '{:.15f}'.format(avg_time_solveRelaxation/ 100))
print("Среднее количество итераций метода релаксации:\t\t", '{:.15f}'.format(avg_operRelax/ 100))
print("Минимальное количество итераций метода релаксации:\t", '{:.15f}'.format(min_operRelax))
print("Максимальное количество итераций метода релаксации:\t", '{:.15f}'.format(max_operRelax))
print("Нормы разности решения методом релаксации с точным решением:")
print("\t Средняя:\t", '{:.15f}'.format(avg_normRelax/ 100))
print("\t Минимальная:\t", '{:.15f}'.format(min_normRelax))
print("\t Максимальная:\t", '{:.15f}'.format(max_normRelax))
