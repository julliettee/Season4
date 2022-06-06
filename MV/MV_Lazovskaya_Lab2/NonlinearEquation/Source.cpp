#include <iostream>
#include <vector>
#include <iomanip>

#define int long long

using namespace std;

const double PI = acos(-1.0);

double func(double x) {
	return (pow(x, 9) + PI) * (cos(log(x * x + 1))) / exp(x * x) - x / 2022;
}
int sign(double x) {
	return (x < 0) * (-2) + 1;
}
int bisection(double& l, double& r, double eps) {
	int k = 0;
	while (r - l > eps) {
		double mid = (r + l) * 0.5;
		k++;
		if (sign(func(mid)) == sign(func(l))) {
			l = mid;
		}
		else {
			r = mid;
		}
	}
	return k;
}

double Newton(double x, double eps) {
	int k = 0;
	double x_new = x;
	x = 1e9 + x;
	double h = 0.00001;
	while (abs(x_new - x) > eps) {
		x = x_new;
		k++;
		double f_pr = (func(x + h) - func(x)) / h;
		x_new = x - func(x) / f_pr;
	}
	cout << "Newton iterations: " << k << "\n";
	return x_new;
}

int32_t main() {
	double l = -2, r = -1.7;
	int k = bisection(l, r, 1e-4);
	cout << "---------------------------------------------\n";
	cout << "Check for #1 \n";
	cout << "Bisection iterations: " << k << "\n";
	cout << "New L & R: " << l << ' ' << r << "\n";
	double solution = Newton((l + r) / 2, 1e-18);
	cout << fixed << setprecision(15) << "Solution: " << solution << "\n";
	cout << fixed << setprecision(15) << "f(x) = " << func(solution) << "\n";
	cout << "---------------------------------------------\n\n";

	l = -1.5, r = -1;
	k =  bisection(l, r, 1e-4);
	cout << "---------------------------------------------\n";

	cout << "Check for #2 \n";
	cout << "New L & R: " << l << ' ' << r << "\n";
	cout << "Bisection iterations: " << k << "\n";
	solution = Newton((l + r) / 2, 1e-18);
	cout << fixed << setprecision(15) << "Solution: " << solution << "\n";
	cout << fixed << setprecision(15) << "f(x) = " << func(solution) << "\n";
	cout << "---------------------------------------------\n\n";


	l = 1.6, r = 2.6;
	k =  bisection(l, r, 1e-4);
	cout << "---------------------------------------------\n";

	cout << "Check for #3 \n";
	cout << "New L & R: " << l << ' ' << r << "\n";
	cout << "Bisection iterations: " << k << "\n";
	solution = Newton((l + r) / 2, 1e-18);
	cout << fixed << setprecision(15) << "Solution: " << solution << "\n";
	cout << fixed << setprecision(15) << "f(x) = " << func(solution) << "\n";
	cout << "---------------------------------------------\n\n";

}