#include <iostream>
#include <vector>
#include <iomanip>

#define int long long

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
double GetNorm(vector <double>& u) {
    double norm = 0;
    for (auto x : u) {
        norm = max(norm, abs(x));
    }
    return norm;
}


StepennoyResult SolveCase3(vector <vector <double>>& A, vector <double>& u_i_1, complex& eigen_value, double eps) {
    int n = u_i_1.size();
    vector <double> v = MultiplyMV(A, u_i_1);
    vector <double> u;
    double norm = GetNorm(v);
    for (int i = 0; i < n; i++) {
        u.push_back(v[i] / norm);
    }

    vector <double> v1 = MultiplyMV(A, u);
    vector <double> u1;
    norm = GetNorm(v1);
    for (int i = 0; i < n; i++) {
        u1.push_back(v1[i] / norm);
    }

    vector <double> v2 = MultiplyMV(A, u1);
    vector <double> u2;
    norm = GetNorm(v2);
    for (int i = 0; i < n; i++) {
        u2.push_back(v2[i] / norm);
    }

    int pos = 0;
    double mx = 0;
    for (int i = 0; i < n; i++) {
        if (v1[i] != 0 &&
            abs(v[i] * v2[i] * GetNorm(v1) - v1[i] * v1[i] * GetNorm(v)) != 0
            and abs(u_i_1[i] * v1[i] - (u[i] * u[i]) * GetNorm(v)) > mx
            and abs(u_i_1[i] * v1[i] - (u[i] * u[i]) * GetNorm(v)) * v1[i] != 0) {
            mx = abs(u_i_1[i] * v1[i] - (u[i] * u[i]) * GetNorm(v));
            pos = i;
        }
    }
    double r = sqrt(abs(v[pos] * v2[pos] * GetNorm(v1) - v1[pos] * v1[pos] * GetNorm(v)) / mx);
    double cos = (v2[pos] * GetNorm(v1) + r * r * u[pos]) / (2 * r * v1[pos]);
    double sin = sqrt((1 - cos * cos));
    complex value = complex(r * cos, r * sin);
    vector <complex> eig_vec(n, complex(0));
    norm = GetNorm(v2);
    for (int i = 0; i < n; i++) {
        eig_vec[i].real = v1[i] - value.real * u[i];
        eig_vec[i].imag = -value.imag * u[i];
        u[i] = v2[i] / norm;
    }

    StepennoyResult res;
    double diff = sqrt((value.real - eigen_value.real) * (value.real - eigen_value.real) + (value.imag - eigen_value.imag) * (value.imag - eigen_value.imag));
    if (diff < eps) {
        res.value3 = value;
        res.eig_vec3.resize(n, complex(0));
        for (int i = 0; i < n; i++) {
            res.eig_vec3[i] = eig_vec[i];
        }
        res.isSolution = true;
    }
    else {
        res.isSolution = false;
    }
    eigen_value = value;
    for (int i = 0; i < n; i++) {
        u_i_1[i] = u2[i];
    }
    return res;

}

StepennoyResult SolveCase2(vector <vector <double>>& A, vector <double>& u, complex& eigen_value, double eps) {
    int n = u.size();
    vector <double>  v1 = MultiplyMV(A, u);
    vector <double>  v2 = MultiplyMV(A, v1);
    int pos = 0;
    for (int i = 0; i < n; i++) {
        if (abs(u[pos]) < abs(u[i])) {
            pos = i;
        }
    }
    double value = sqrt(abs(v2[pos]/u[pos]));
    vector <double> eig_vec(n, 0);
    for (int i = 0; i < n; i++) {
        eig_vec[i] = v2[i] + value * u[i];
    }

    double norm = GetNorm(v2);
    for (int i = 0; i < n; i++) {
        v2[i] /= norm;
    }
    StepennoyResult res;
    if (abs(value - eigen_value.real) < eps) {
        res.isSolution = true;
        res.eig_vec.resize(n, 0);
        for (int i = 0; i < n; i++) {
            res.eig_vec[i] = u[i];
        }
        res.value = value;
    }
    else {
        res.isSolution = false;
    }
    eigen_value.real = value;
    for (int i = 0; i < n; i++) {
        u[i] = v2[i];
    }
    return res;
}

StepennoyResult SolveCase1(vector <vector <double>> &A, vector <double>& u, complex& eigen_value, double eps) {
    int n = u.size();
    vector <double>  v = MultiplyMV(A, u);
    int pos = 0;
    for (int i = 0; i < n; i++) {
        if (abs(u[pos]) < abs(u[i])) {
            pos = i;
        }
    }
    double value = v[pos] / u[pos];
    // нормируем вектор
    double norm = GetNorm(v);
    for (int i = 0; i < n; i++) {
        u[i] = v[i] / norm;
    }
    StepennoyResult res;
    if (abs(value - eigen_value.real) < eps) {
        res.isSolution = true;
        res.eig_vec.resize(n, 0);
        for (int i = 0; i < n; i++) {
            res.eig_vec[i] = u[i];
        }
        res.value = value;
    }
    else {
        res.isSolution = false;
    }
    eigen_value.real = value;
    return res;
}



void StepennoyMethod(vector <vector <double>> A, vector<double> y, double eps, bool isStat) {
    int n = y.size();
    vector <complex> eigen_values(3, 1e9 + 7);
    vector <double> vecs_last[3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < n; j++) {
            vecs_last[i].push_back(y[j]);
        }
    }
    StepennoyResult(*funcs[3])(vector <vector <double>> &A, vector <double> &y, complex& eigen_value, double eps) = { SolveCase1, SolveCase2, SolveCase3 };
    bool cases[3] = { true, true, true };
    bool isFind = false;
    while (!isFind && (cases[0] || cases[1] || cases[2])) {
        for (int i = 0; i < 3; i++) {
            if (cases[i]) {
                StepennoyResult res = (funcs[i])(A, vecs_last[i], eigen_values[i], eps);
                if (res.isSolution) {
                    isFind = true;
                    if (!isStat) {
                        if (i != 2) {
                            cout << "Case #" << i + 1 << "\n";
                            cout << "Value = " << fixed << setprecision(11) << res.value << "\n";
                            cout << "Vector: ";
                            for (auto to : res.eig_vec) {
                                cout << fixed << setprecision(9) << to << "\n";
                            }
                            cout << "\n";
                        }
                        else {
                            cout << "Case #" << i + 1 << "\n";
                            cout << "Value = " <<  fixed << setprecision(11) << res.value3.real << " + " << res.value3.imag << "i\n";
                            cout << "Vector: \n";
                            for (complex to : res.eig_vec3) {
                                cout << fixed << setprecision(9) << to.real << " + " << to.imag << "i \n";
                            }
                            cout << "\n";
                        }
                        
                    }
                   
                }
            }
        }
    }

}

int32_t main() {
   // vector <vector<double>> matrix = { {-7, 12, 5}, {-5, 10, 5}, {-12, 12, 10} };
    //vector <vector<double>> matrix = { {18, -8, -20}, {20, -10, -20}, {8, -8, -10} };


    vector <vector <double>> A1{
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



    vector <double> ya = {1, -1, 1, 1, 1, -1,2, -2,1, 2};
    StepennoyMethod(A1, ya, 0.000000001, false);
    vector <double> yb = { 1, -1, 1, 1, 1, -1,2, -2,1, 2, 1, -1, 1, 1, 1, -1,2, -2,1, 2 };
    StepennoyMethod(B1, yb, 0.000000001, false);

    cout << "Statistic? y/n \n";
    char с;
    if (с == 'y') {
        srand(time(0));
        vector <double> stats(201, 0);
        for (int n = 2; n <= 200; n++) {
            if (n % 10 == 0) cout << "n = " << n << "\n";
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
            StepennoyMethod(C, yc, 0.0000001, true);
            double t_finish = clock() * 1.0;
            stats[n] = (t_finish - t_start) / CLOCKS_PER_SEC;
            //cout << stats[n] << "\n";
        }
        for (int i = 2; i < 201; i++) {
            cout << i << ", ";
        }
        cout << "\n\n";
        for (int i = 2; i < 201; i++) {
            cout << stats[i] << ", ";
        }
    }




}