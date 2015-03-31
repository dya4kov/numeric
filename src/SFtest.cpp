#include "../hdr/SF/FermiDirac.h"
#include "../hdr/numericTypes.h"
#include <iostream>

int main() {
	Double x1 = -50.0;
	Double x2 = 100.0;
	Double dx = 1.0;
	Double x = 0;
	Double n = (x2 - x1)/dx;
	Double mhalf = 0;
	Double half = 0;
	Double three_half = 0;
	for (x = x1; x <= x2; x += dx) {
		std::cout << "x = " << x; 
		std::cout << ", FD_mhalf(x) = " << FD_mhalf(x);
		std::cout << ", FD_mhalf(x) = " << FD_half(x);
		std::cout << ", FD_mhalf(x) = " << FD_3half(x);
		std::cout << std::endl;
	}
	return 0;
}