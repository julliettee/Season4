#include <iostream>
#include <vector>
#include <fstream>
#define timer (clock() * 1.0 / CLOCKS_PER_SEC)

double st = double(5.0) / 4.0;
int operation = 1;

using namespace std;

double midFindInverseMatrix = 0;
double midc_A = 0, maxc_A = 0, minc_A = 1e9 + 7;
double midFindGaussSolution = 0;
double midFindLUPDecomposition = 0;
double midFindLUPSolution = 0;
double midFindSolutionSQRT = 0;
double midFindsolutionRelax = 0;
double minFindsolutionRelax = 1e9 + 7;
double maxFindsolutionRelax = 0;

double midNormGauss = 0;
double minNormGauss = 1e9 + 7;
double maxNormGauss = 0;

double midNormLUP = 0;
double minNormLUP = 1e9 + 7;
double maxNormLUP = 0;

double midNormSQRT = 0;
double minNormSQRT = 1e9 + 7;
double maxNormSQRT = 0;

double midNormRelax = 0;
double minNormRelax = 1e9 + 7;
double maxNormRelax = 0;



int minIter = 1e9 + 7, maxIter = 0;
double midIter = 0;

int main() {

	double l = -pow(2.0, st);
	double r = pow(2.0, st);
	const int N = 256;


	// printf("%f, %f, st = %f", l, r, st);
	while (operation--) {
		printf("%d\n", operation);
		vector <double> A[N];
		for (int i = 0; i < N; i++) {
			A[i].resize(N, 0);
		}
		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {

				double cur = l + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (r - l)));
				A[i][j] = cur;
				A[j][i] = cur;
			}
		}
		for (int i = 0; i < N; i++) {
			double s = 0;
			for (int j = 0; j < N; j++) {
				s += abs(A[i][j]);
			}
			A[i][i] = s;
		}
		/*for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%f ", A[i][j]);
			}
			printf("\n");
		}*/
		vector <double> x(N);
		for (int i = 0; i < N; i++) {
			x[i] = l + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (r - l)));

		}

		// Ay = b
		vector <double> b(N, 0);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				b[i] += A[i][j] * x[j];
			}
		}
		/*printf("Real solution = (");
		for (int i = 0; i < N; i++) {
			if (i != N - 1) {
				printf("%f, ", x[i]);
			}
			else {
				printf("%f)\n", x[i]);
			}
		}*/

		double T = clock() * 1.0 / CLOCKS_PER_SEC;
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
		double T_inversiveA = timer - T;
		midFindInverseMatrix += T_inversiveA;
		// printf("Time inverse matrix: %f\n", T_inversiveA);
#pragma endregion 
		/*for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%f ", A[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%f ", inverseA[i][j]);
			}
			printf("\n");
		}*/
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
		minc_A = min(minc_A, c_A);
		maxc_A = max(maxc_A, c_A);
		midc_A += c_A;
#pragma endregion
#pragma region методGauss
		T = timer;
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
		double T_Gauss = timer - T;
		midFindGaussSolution += T_Gauss;
		double norm = 0;
		for (int i = 0; i < N; i++) {
			norm = max(norm, abs(x[i] - xg[i]));
		}
		midNormGauss += norm;
		minNormGauss = min(minNormGauss, norm);
		maxNormGauss = max(maxNormGauss, norm);
#pragma endregion

#pragma region построение_LUP
		T = timer;
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
		double T_FindLUPDecomposition = timer - T;
		midFindLUPDecomposition += T_FindLUPDecomposition;
		// printf("Find LUP-decomposition time: %f\n", T_FindLUPDecomposition);

#pragma endregion 
#pragma region FindSolutionWithLUP
		T = timer;
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
		double T_FindLUPSolution = timer - T;
		midFindLUPSolution += T_FindLUPDecomposition;
		// printf("Find LUP-solution time: %f\n", T_FindLUPSolution);
		norm = 0;
		for (int i = 0; i < N; i++) {
			norm = max(norm, abs(x[i] - xLUP[i]));
		}
		midNormLUP += norm;
		minNormLUP = min(minNormLUP, norm);
		maxNormLUP = max(maxNormLUP, norm);
		/*printf("LUP solution = (");
		for (int i = 0; i < N; i++) {
			if (i != N - 1) {
				printf("%f, ", xLUP[i]);
			}
			else {
				printf("%f)\n", xLUP[i]);
			}
		}*/



#pragma endregion

#pragma region LDLT_SQRT

		T = timer;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++){
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
		double T_FindSolutionSQRT = timer - T;
		midFindSolutionSQRT += T_FindSolutionSQRT;
		norm = 0;
		for (int i = 0; i < N; i++) {
			norm = max(norm, abs(x[i] - xSQRT[i]));
		}
		midNormSQRT += norm;
		minNormSQRT = min(minNormSQRT, norm);
		maxNormSQRT = max(maxNormSQRT, norm);
#pragma endregion
		
#pragma region Relax

		T = timer;
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
		double T_FindSolutionRelax = timer - T;
		// printf("Find solution relax time: %f\n", T_FindSolutionRelax);
		midFindsolutionRelax += T_FindSolutionRelax;

		minIter = min(minIter, iter);
		maxIter = max(maxIter, iter);
		midIter += iter;
		norm = 0;
		for (int i = 0; i < N; i++) {
			norm = max(norm, abs(x[i] - x1[i]));
		}
		midNormRelax += norm;
		minNormRelax = min(minNormRelax, norm);
		maxNormRelax = max(maxNormRelax, norm);
		/*printf("iter = %d\n", iter);
		printf("Relax solution = (");
		for (int i = 0; i < N; i++) {
			if (i != N - 1) {
				printf("%f, ", x1[i]);
			}
			else {
				printf("%f)\n", x1[i]);
			}
		}*/

#pragma endregion



	}
	printf("Middle ||A^-1||*||A||: %.13f\t\t\n", (midc_A / 100));
	printf("Min ||A^-1||*||A|: %.13f:\t\t\t\n", (minc_A));
	printf("Max ||A^-1||*||A|: %.13f:\t\t\t\n", (maxc_A));
	printf("Average search time of inverse matrix: %.13f\t\t\n", (midFindInverseMatrix / 100));
	printf("Average solution time with Gauss' method: %.13f\t\t\n", (midFindGaussSolution / 100));
	printf("Norms by Gauss method:");
	printf("\t Middle norm: %.13f\t\n", (midNormGauss / 100));
	printf("\t Min norm: %.13f\t\n", (minNormGauss));
	printf("\t Max norm: %.13f\t\n", (maxNormGauss));
	printf("Average build time of LUP: %.13f\t\t\n", (midFindLUPDecomposition / 100));
	printf("Average solution time LUx = b1: %.13f\t\t\n", (midFindLUPSolution / 100));
	printf("Norms with LUP decomposition:\n");
	printf("\t Middle norm: %.13f\t\n", (midNormLUP / 100));
	printf("\t Min norm: %.13f\t\n", (minNormLUP));
	printf("\t Max norm: %.13f\t\n", (maxNormLUP));
	printf("Average solution time with SQRT method: %.13f\t\n", (midFindSolutionSQRT / 100));
	printf("Norms with SQRT:\n");
	printf("\t Middle norm: %.13f\t\n", (midNormSQRT / 100));
	printf("\t Min norm: %.13f\t\n", (minNormSQRT));
	printf("\t Max norm: %.13f\t\n", (maxNormSQRT));
	printf("Average solution time with Relaxation method: %.13f\t\n", (midFindsolutionRelax / 100));
	printf("Middle amount of iterations with Relaxation method: %.5f\t\n", (1.0 * midIter / 100.0));
	printf("Min amount of iterations with Relaxation method: %d\t\n", (minIter));
	printf("Max amount of iterations with Relaxation method: %d\t\n", (maxIter));
	printf("Norms with Relaxation method:\n");
	printf("\t Middle norm: %.13f\t\n", (midNormRelax / 100));
	printf("\t Min norm: %.13f\t\n", (minNormRelax));
	printf("\t Max norm: %.13f\t\n", (maxNormRelax));
	

	
	return 0;
}