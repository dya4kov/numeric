#include "../hdr/SF/FermiDirac.h"
#include "../hdr/numericTypes.h"
#include <iostream>

int main() {
	Double x1 = -50.0;
	Double x2 = 100.0;
	Double dx = 1.0;
	Double x = 0;
	Double n = (x2 - x1)/dx;
	FermiDirac<DMhalf> FDdmhalf;
	FermiDirac<Mhalf> FDmhalf;
	FermiDirac<Half> FDhalf;
	FermiDirac<ThreeHalf> FD3half;
	for (x = x1; x <= x2; x += dx) {
		std::cout << "x = " << x; 
		std::cout << ", FD_dmhalf(x) = " << FDdmhalf(x);
		std::cout << ", FD_mhalf(x) = " << FDmhalf(x);
		std::cout << ", FD_half(x) = " << FDhalf(x);
		std::cout << ", FD_3half(x) = " << FD3half(x);
		std::cout << std::endl;
	}
	return 0;
}