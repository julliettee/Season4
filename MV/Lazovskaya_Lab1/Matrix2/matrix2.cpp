#include <iostream>
#include <vector>
#include <fstream>
#define timer (clock() * 1.0 / CLOCKS_PER_SEC)

double st = double(5.0) / 4.0;

using namespace std;

int main() {
	const int N = 8;
	vector <double> A_[N], A_T[N];
	double l = -pow(2.0, st);
	double r = pow(2.0, st);
	ifstream fin("matrix2.txt");
	for (int i = 0; i < N; i++) {
		A_[i].resize(N);
		for (int j = 0; j < N; j++) {
			fin >> A_[i][j];
		}
	}
	fin.close();
	vector <double> x(N);
	for (int i = 0; i < N; i++) {
		A_T[i].resize(N);
		for (int j = 0; j < N; j++) {
			A_T[i][j] = A_[i][j];
		}
	}
	vector <double> A[N];
	for (int i = 0; i < N; i++) {
		A[i].resize(N);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int l = 0; l < N; l++) {
				A[i][j] += A_T[i][l] * A_[l][j];
			}
		}
	}
	for (int i = 0; i < N; i++) {
		x[i] = l + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (r - l)));
	}
	vector <double> b(N, 0);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			b[i] += A[i][j] * x[j];
		}
	}
#pragma region поиск_обратной_матрицы_inverseA
	vector <double> A2[N];
	for (int i = 0; i < N; i++) {
		A2[i].resize(N * 2, 0);
		for (int j = 0; j < N; j++) {
			A2[i][j] = A[i][j];
		}
		A2[i][N + i] = 1;
	}
	// приведение к верхнетреугольному виду
	for (int i = 0; i < N - 1; i++) {
		for (int j = i + 1; j < N; j++) {
			double m = A2[i][j] / A2[i][i];
			for (int k = i; k < A2[i].size(); k++) {
				A2[j][k] -= m * A2[i][k];
			}
		}
	}

	// приведение к диагональному виду
	for (int i = N - 1; i >= 0; i--) {
		for (int k = i - 1; k >= 0; k--) {
			double m = A2[k][i] / A2[i][i];
			A2[k][i] = 0;
			for (int j = N; j < 2 * N; j++) {
				A2[k][j] -= A2[i][j] * m;
			}
		}
	}

	vector <double> inverseA[N];
	for (int i = 0; i < N; i++) {
		inverseA[i].resize(N, 0);
		double m = A2[i][i];
		for (int j = N; j < 2 * N; j++) {
			inverseA[i][j - N] = A2[i][j] / m;
		}
	}
#pragma endregion 
#pragma region поиск_числа_обусловленности
	double normA = 0, normInverseA = 0;
	for (int i = 0; i < N; i++) {
		double s1 = 0, s2 = 0;
		for (int j = 0; j < N; j++) {
			s1 += abs(A[i][j]);
			s2 += abs(inverseA[i][j]);
		}
		normA = max(normA, s1);
		normInverseA = max(normInverseA, s2);
	}

	double c_A = normA * normInverseA;
	printf("Cond A1 = %.13f\n", c_A);

#pragma endregion
#pragma region методGauss
	vector <double> G[N];
	vector <double> bg(N, 0);
	for (int i = 0; i < N; i++) {
		G[i].resize(N, 0);
		bg[i] = b[i];
		for (int j = 0; j < N; j++) {
			G[i][j] = A[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		double mainE = abs(G[i][i]);
		int pos = i;
		for (int j = i + 1; j < N; j++) {
			if (abs(G[j][i]) > mainE) {
				mainE = abs(G[j][i]);
				pos = j;
			}
		}
		for (int j = 0; j < N; j++) {
			swap(G[i][j], G[pos][j]);
		}
		swap(bg[i], bg[pos]);
		for (int j = i + 1; j < N; j++) {
			double m = G[j][i] / G[i][i];
			for (int k = i; k < N; k++) {
				G[j][k] -= m * G[i][k];
			}
			bg[j] -= bg[i] * m;
		}
	}
	vector <double> xg(N, 0);
	for (int i = N - 1; i >= 0; i--) {
		double withoutXi = 0;
		for (int j = i + 1; j < N; j++) {
			withoutXi += xg[j] * G[i][j];
		}
		xg[i] = (bg[i] - withoutXi) / G[i][i];
	}
	double norm = 0;
	printf("GAUSS solution = (");
	for (int i = 0; i < N; i++) {
		if (i != N - 1) {
			printf("%.13f, ", xg[i]);
		}
		else {
			printf("%.13f)\n", xg[i]);
		}
	}
	printf("Norm Gauss = %.13f\n", norm);

#pragma endregion

#pragma region построение_LUP
	vector <double> U[N], L[N];
	for (int i = 0; i < N; i++) {
		U[i].resize(N, 0);
		for (int j = 0; j < N; j++) {
			U[i][j] = A[i][j];
		}
		L[i].resize(N, 0);
		L[i][i] = 1;
	}
	vector < pair <int, int> > permutations;
	vector <int> finalP;
	for (int i = 0; i < N; i++) {
		double mainE = abs(U[i][i]);
		int pos = i;
		finalP.push_back(i);
		for (int j = i + 1; j < N; j++) {
			if (abs(U[j][i]) > mainE) {
				mainE = abs(U[j][i]);
				pos = j;
			}
		}
		permutations.push_back({ i, pos });
		for (int j = 0; j < N; j++) {
			swap(U[i][j], U[pos][j]);
		}
		for (int j = i + 1; j < N; j++) {
			double m = U[j][i] / U[i][i];
			L[j][i] = m;
			for (int k = i; k < N; k++) {
				U[j][k] -= m * U[i][k];
			}
		}
	}
	for (int i = 0; i < N; i++) {
		swap(finalP[permutations[i].first],
			finalP[permutations[i].second]);
		for (int j = i + 1; j < N; j++) {
			swap(L[permutations[j].first][i],
				L[permutations[j].second][i]);
		}
	}

#pragma endregion 
#pragma region FindSolutionWithLUP
	vector <double> bLUP(N), y(N, 0), xLUP(N, 0);
	for (int i = 0; i < N; i++) {
		bLUP[i] = b[finalP[i]];
	}
	for (int i = 0; i < N; i++) {
		double withoutYi = 0;
		for (int j = 0; j < i; j++) {
			withoutYi += y[j] * L[i][j];
		}
		y[i] = ((bLUP[i] - withoutYi) / L[i][i]);
	}

	for (int i = N - 1; i >= 0; i--) {
		double withoutXi = 0;
		for (int j = i + 1; j < N; j++) {
			withoutXi += xLUP[j] * U[i][j];
		}
		xLUP[i] = (y[i] - withoutXi) / U[i][i];
	}
	norm = 0;
	for (int i = 0; i < N; i++) {
		norm = max(norm, abs(x[i] - xLUP[i]));
	}
	printf("LUP norm = %.13f\n", norm);
	printf("LUP solution = (");
	for (int i = 0; i < N; i++) {
		if (i != N - 1) {
			printf("%.13f, ", xLUP[i]);
		}
		else {
			printf("%.13f)\n", xLUP[i]);
		}
	}



#pragma endregion

#pragma region LDLT_SQRT

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			L[i][j] = A[i][j];
		}
	}


	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			double m = L[j][i] / L[i][i];
			for (int k = 0; k < N; k++) {
				L[j][k] -= m * L[i][k];
			}
		}
		for (int j = i + 1; j < N; j++) {
			L[i][j] /= (L[i][i]);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			L[i][j] *= sqrt(abs(L[i][i]));
			L[i][j] *= abs(L[i][i]) / L[i][i];
		}
		L[i][i] /= sqrt(abs(L[i][i]));
	}

	y.assign(N, 0);


	for (int i = 0; i < N; i++) {
		double withoutYi = 0;
		for (int j = 0; j < i; j++) {
			withoutYi += y[j] * L[j][i];
		}
		y[i] = ((b[i] - withoutYi) / L[i][i]);
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			L[i][j] *= abs(L[i][i]) / L[i][i];
		}
		L[i][i] *= abs(L[i][i]) / L[i][i];
	}
	vector <double> xSQRT(N, 0);
	for (int i = N - 1; i >= 0; i--) {
		double withoutXi = 0;
		for (int j = i + 1; j < N; j++) {
			withoutXi += xSQRT[j] * L[i][j];
		}
		xSQRT[i] = (y[i] - withoutXi) / L[i][i];
	}

	norm = 0;
	for (int i = 0; i < N; i++) {
		norm = max(norm, abs(x[i] - xSQRT[i]));
	}
	printf("SQRT norm = %.13f\n", norm);
	printf("SQRT solution = (");
	for (int i = 0; i < N; i++) {
		if (i != N - 1) {
			printf("%.13f, ", xSQRT[i]);
		}
		else {
			printf("%.13f)\n", xSQRT[i]);
		}
	}

#pragma endregion

#pragma region Relax
	double w = 1.0 - double(5.0) / 40.0;
	double eps = 1e-3;
	vector <double> x0(N, 0), x1(N, 0);
	vector <double> difference(N, 0);
	vector <double> bR(N, 0);
	vector <double> AR[N];
	for (int i = 0; i < N; i++) {
		AR[i].resize(N, 0);

	}
	int iter = 0;

	while (true) {
		iter++;
		for (int i = 0; i < N; i++) {
			double s0 = 0, s1 = 0;
			for (int j = 0; j < i; j++) {
				AR[i][j] = -A[i][j] * (w / A[i][i]);
				s1 += AR[i][j] * x1[j];
			}
			for (int j = i + 1; j < N; j++) {
				AR[i][j] = -A[i][j] * (w / A[i][i]);
				s0 += AR[i][j] * x0[j];
			}
			bR[i] = (b[i] * w) / A[i][i];
			x1[i] = (s0 + s1 + bR[i]) - x0[i] * (w - 1);
		}
		for (int i = 0; i < N; i++) {
			difference[i] = x1[i] - x0[i];
		}
		for (int i = 0; i < N; i++) {
			x0[i] = x1[i];
		}
		double norm = 0;
		for (auto to : difference) {
			norm = max(norm, abs(to));
		}
		if (norm < eps) {
			break;
		}
	}

	norm = 0;
	for (int i = 0; i < N; i++) {
		norm = max(norm, abs(x[i] - x1[i]));
	}

	printf("iter = %d\n", iter);
	printf("Relax norm = %.13f\n", norm);
	printf("Relax solution = (");
	for (int i = 0; i < N; i++) {
		if (i != N - 1) {
			printf("%.13f, ", x1[i]);
		}
		else {
			printf("%.13f)\n", x1[i]);
		}
	}

#pragma endregion
	return 0;
}