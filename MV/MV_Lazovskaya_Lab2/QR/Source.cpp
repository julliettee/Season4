#include <iostream>
#include <vector>
#include <iomanip>

#define int long long
#define double long double


using namespace std;

class complex {
public:
    double real, imag;
    complex() {
        real = 0, imag = 0;
    }
    complex(double a, double b = 0) {
        real = a, imag = b;
    }
};


struct StepennoyResult {
    bool isSolution;
    vector <double> eig_vec;
    vector <complex> eig_vec3;
    double value;
    complex value3;
};


complex mult(double val, complex& M) {
    return complex(double(M.real * val), double(M.imag * val));
}
complex mult(complex& M, double val) {
    return complex(M.real * val, M.imag * val);
}
complex sum(complex a, complex b) {
    return complex(a.real + b.real, a.imag + b.imag);
}

vector <double>  MultiplyMV(vector <vector <double> > S, vector <double> b) {
    int n = b.size();
    vector <double>  C;

    C.resize(n, 0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i] += S[i][j] * b[j];

        }
    }
    return C;
}


vector <vector <double>>  MultiplyMM(vector <vector <double> > S, vector < vector <double >> B) {
    int n = B.size();
    vector <vector <double>>  C;

    C.resize(n);
    for (int i = 0; i< n; i++) {
        C[i].resize(n, 0);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int l = 0; l < n; l++) {
                C[i][j] += S[i][l] * B[l][j];
            }

        }
    }
    return C;
}


vector <vector <double>>  Trans(vector <vector <double> > S) {
    int n = S.size();
    vector <vector <double>>  C;

    C.resize(n);
    for (int i = 0; i < n; i++) {
        C[i].resize(n, 0);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = S[j][i];
        }
    }
    return C;
}

vector <vector <double>>  E(int n) {
    vector <vector <double>>  C;
    C.resize(n);
    for (int i = 0; i < n; i++) {
        C[i].resize(n, 0);
        C[i][i] = 1;
    }
    return C;
}


double GetNorm(vector <double>& u) {
    double norm = 0;
    for (auto x : u) {
        norm = max(norm, abs(x));
    }
    return norm;
}




void Waiting(vector <vector <double>> &matrix) {
    int n = matrix.size();
    vector <vector <double>> Q = E(n);
    for (int j = 0; j < n - 1; j++) {
        int i = j + 1;
        double a = matrix[j][j] * matrix[j][j], b = matrix[i][j] * matrix[i][j];
        if (a + b == 0) {
            continue;
        }
        double determinator = sqrt(a + b);
        double s = matrix[i][j] / determinator;
        double c = matrix[j][j] / determinator;

        //matrix = MultiplyMM(Qj, matrix);
        for (int l = 0; l < n; l++) {
            double was = matrix[j][l];
            matrix[j][l] = c * matrix[j][l] + s * matrix[i][l];
            matrix[i][l] = -s * was + c * matrix[i][l];
        }

        for (int l = 0; l < n; l++) {
            double was = Q[l][j];
            Q[l][j] = c * Q[l][j] + s * Q[l][i];
            Q[l][i] = -s * was + c * Q[l][i];
        }
       
    }
    matrix = MultiplyMM(matrix, Q);
    /*double norm = 0;
    for (int i = 0; i < n; i++) {
        for (auto x : matrix[i]) {
            norm += x * x;
        }
    }
    norm = sqrt(norm);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] /= norm;
        }
    }*/

}

int Counter(vector <vector <double>>& matrix) {
    int n = matrix.size();
    double eps = 1e-11;
    int count = 0;
    for (int i = 0; i + 1 < n; i++) {
        if (abs(matrix[i + 1][i]) < eps) {
            count++;
        }
    }
    return count;
}


vector <complex> QR(vector <vector <double>>& matrix) {
    double eps = 1e-18;
    int n = matrix.size();
    vector <complex> eigh_values;
    for (int i = 0; i < n; ) {
        if ((i + 1 < n && abs(matrix[i + 1][i]) < eps) || i == n - 1) {
            eigh_values.push_back(complex(matrix[i][i], 0));
            i++;
        }
        else {
            double b = (matrix[i][i] + matrix[i + 1][i + 1]);
            double ac4 = 4 * (matrix[i][i] * matrix[i + 1][i + 1] - matrix[i + 1][i] * matrix[i][i + 1]);
            double Discream = b * b - ac4;
            if (Discream < 0) {
                complex value1 = complex(b / 2, sqrt(abs(Discream)) / 2);
                complex value2 = complex(b / 2, -sqrt(abs(Discream)) / 2);
                eigh_values.push_back(value1);
                eigh_values.push_back(value2);
            }
            else {
                return {};
            }
            i+=2;
        }
    }
    return eigh_values;
}

void print(vector < vector <double> >& matrix) {
    cout << "[";
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        cout << "[";
        for (int j = 0; j < n; j++) {
            if (j < n - 1) {
                cout << matrix[i][j] << ", ";
            }
            else {
                cout << matrix[i][j] << "]";
            }
        }
        cout << "," << endl;
    }
    cout << endl;
}
void Hessenberg(vector < vector <double> >& matrix) {
    //print(matrix);
    int n = matrix.size();
    double eps = 1e-13;
    for (int i = n - 2; i; i--) { // перебираем столбец, с помощью которого зануляем
        for (int j = i - 1; j >= 0; j--) { // перебираем столбец в котором зануляем i+1 строку
            //cout << "i j = " << i << ' ' << j << endl;
            if (abs(matrix[i + 1][i]) < eps) continue;
            double k = matrix[i + 1][j]/matrix[i + 1][i];
            for (int l = 0; l < n; l++) {
                matrix[l][j] -= matrix[l][i] * k; // от j отнимаем i * k
                if ((abs(matrix[l][j]) < eps)) {
                    matrix[l][j] = 0;
                }
            }
            //print(matrix);
            for (int l = 0; l < n; l++) {
                matrix[i][l] += matrix[j][l] * k;
                if ((abs(matrix[i][l]) < eps)) {
                    matrix[i][l] = 0;
                }
            }
            //print(matrix);
        }
    }
   // print(matrix);
}

vector <complex> Solve(vector < vector <double> > &matrix) {
    int n = matrix.size();
    int k = 0;
    Hessenberg(matrix);
    while (Counter(matrix) < (n - 1) / 2 && k < 5000) {
        k++;
        Waiting(matrix);
        Hessenberg(matrix);
    }
    vector <complex> cur = QR(matrix);
    while (cur.empty()) {
        Waiting(matrix);
        cur = QR(matrix);
    }
    cout << "wait\n";
    double eps = 1e-14;
    double fault = 1e9;
    k = 0;
    while (fault > eps || k < 5000) {
        Waiting(matrix);
        k++;
        vector <complex> new_cur = QR(matrix);
        if (new_cur.size() == 0) continue;
        fault = 0;
        for (int i = 0; i < n; i++) {
            complex difference =
                complex(new_cur[i].real - cur[i].real, new_cur[i].imag - cur[i].imag);
            double modul = sqrt(difference.real * difference.real + difference.imag * difference.imag);
            cur[i] = new_cur[i];
            fault = max(fault, modul);
        }
    }
    return cur;
}

int32_t main() {
 
    vector <vector <double>> A{
       {1, -2, 1, 0, -1, 1, -2, 2, 0, -2},
    {0, 2, 0, 0, 2, 1, -1, -1, -1, -2},
    {0, 1, 0, -1, 1, -1, 0, -1, 1, -1},
    {-2, -1, 2, -1, 0, 0, 0, 0, 1, 0},
    {1, -2, 0, 1, 0, -2, -1, 0, 2, 2},
    {-2, -2, 0, -2, 0, 1, 1, -2, 1, 1},
    {-1, -2, -1, -1, -2, -1, -2, 1, -1, 2},
    {-2, 1, 2, -2, 0, 2, 1, -1, -2, 2},
    {0, 1, 0, 1, 1, -2, 2, 0, 1, 1},
    {0, 0, 2, -1, -1, 0, -2, 2, -1, -1}
    };

    vector <complex> eig_vec1 = Solve(A);
    cout << "Values: \n";
    for (complex to : eig_vec1) {
        cout << fixed << setprecision(11) << to.real << " + " << to.imag << "i \n";
    }
    cout << "\n";

    vector <vector <double>> B1 = {
        {-1, 1, -1, 0, -1, 0, -1, 1, 1, -1, 0, -1, -1, 1, 0, 0, 1, 1, 1, 1},
            {-1, 0, -1, 1, -1, 0, 0, 0, 0, -1, 0, 0, -1, 1, 0, -1, 1, -1, -1, 0},
            {1, 0, -1, 1, 0, 1, -1, -1, -1, 0, -1, -1, 1, -1, 1, 1, -1, 1, -1, 0},
            {-1, 1, 0, 0, -1, 0, 0, -1, 0, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 0},
            {1, 0, -1, 0, 0, -1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, -1, 0, 0, 1},
            {0, 0, 0, 0, -1, 1, 1, 0, 0, 1, 1, 0, -1, 0, 1, 1, 0, 1, 0, 0},
            {-1, 0, 1, 1, 1, -1, -1, 0, -1, 1, -1, -1, -1, 0, -1, 0, 0, 0, -1, 1},
            {0, 0, -1, -1, 0, 1, 1, 1, 1, -1, 0, 0, -1, 1, 1, 1, 1, 0, 0, -1},
            {0, 0, 1, 1, 0, 1, 1, 0, 1, -1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
            {0, -1, 0, 0, 1, 0, -1, 0, -1, 0, -1, 0, -1, 0, 1, -1, 0, 0, 1, 1},
            {1, -1, 1, -1, -1, -1, 1, 0, -1, 0, 1, 1, -1, 0, 1, 1, 1, 0, 0, 0},
            {0, 1, 0, 0, -1, 0, 1, 0, 1, 0, 0, 1, 1, -1, -1, 0, -1, 1, 1, -1},
            {-1, -1, -1, -1, 0, 1, -1, 0, 0, -1, 0, 0, 0, 1, 1, 0, 0, 0, -1, 0},
            {-1, 0, 1, 0, -1, 0, 0, 1, -1, 1, 1, -1, 1, 1, 1, -1, 1, -1, -1, 0},
            {1, -1, 0, -1, -1, 0, -1, -1, 0, 0, 1, 0, 1, 1, -1, 1, 0, 0, -1, 0},
            {-1, -1, 1, 0, -1, 1, 1, -1, 1, 0, 0, -1, 1, -1, -1, 0, 0, 1, 1, 1},
            {0, 0, -1, 0, 0, 0, 0, -1, 1, 1, 0, -1, 1, -1, 0, 0, 0, -1, -1, 1},
            {-1, 0, -1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, -1, 1, 1, 0, -1, 0, -1},
            {-1, 0, 1, 0, 0, 0, 0, -1, 1, -1, 1, -1, 0, -1, -1, 1, 0, 1, 0, 0},
            {0, -1, -1, 1, -1, 1, -1, -1, -1, 1, 1, -1, 0, -1, -1, 0, 1, 0, -1, -1}
    };
    

    vector <complex> eig_vec2 = Solve(B1);
    cout << "Values: \n";
    for (complex to : eig_vec2) {
        cout << fixed << setprecision(11) << to.real << " + " << to.imag << "i \n";
    }
    cout << "\n";

    cout << "Statistic? y/n \n";
    char x;
    cin >> x;
    if (x == 'y') {
        srand(time(0));
        vector <double> stats(26, 0);
        for (int n = 2; n <= 25; n++) {
            cout << "n = " << n << "\n";
            vector <vector < double>> C;
            C.resize(n);
            vector <double> yc(n, 0);
            for (int i = 0; i < n; i++) {
                yc[i] = (-5 + (rand() % 10));
                for (int j = 0; j < n; j++) {
                    C[i].push_back(-5 + (rand() % 10));
                }
            }
            double t_start = clock() * 1.0;
            vector <complex> eig_vec1 = Solve(C);
            double t_finish = clock() * 1.0;
            stats[n] = (t_finish - t_start) / CLOCKS_PER_SEC;
            //cout << stats[n] << "\n";
        }
        for (int i = 2; i < 26; i++) {
            cout << i << ", ";
        }
        cout << "\n\n";
        for (int i = 2; i < 26; i++) {
            cout << stats[i] << ", ";
        }
    }









}