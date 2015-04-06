#include "../hdr/numericTypes.h"
#include <iostream>

int main() {
	Int nrows = 4;
	Int ncols = 4;
	Matrix m(4.0, nrows, ncols);
	m *= 2.0;
	m[1][0] = 1;
	m[0][1] = 2;
	for (int i = 0; i < ncols; ++i) {
		for (int j = 0; j < nrows; ++j) {
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
	nrows = 20;
	ncols = 4;
	std::cout << "resize: " << std::endl;
	m.resize(nrows, ncols);
	for (int i = 0; i < nrows; ++i) {
		for (int j = 0; j < ncols; ++j) {
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
	// Matrix m1 = m;
	// for (int i = 0; i < nrows; ++i) {
	// 	for (int j = 0; j < ncols; ++j) {
	// 		std::cout << m[i][j] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// Int n = 10;
	// Vector v(3.0, n);
	// v *= 2.0;
	// for (int i = 0; i < n; ++i) {
	// 	std::cout << v[i] << std::endl;
	// }
	// n = 20;
	// v.resize(n);
	// std::cout << "resize: " << std::endl;
	// for (int i = 0; i < n; ++i) {
	// 	std::cout << v[i] << std::endl;
	// }
	// Vector &v1 = v;
	// std::cout << "&v: " << &v << std::endl;
	// std::cout << "&v1: " << &v1 << std::endl;
	return 0;
}