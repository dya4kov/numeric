#include "../hdr/ODE/ODEsolver.h"
#include "../hdr/ODE/steppers/ODEstepperPD853.h"
#include "../hdr/ODE/steppers/ODEstepperPD5.h"
#include "../hdr/numericTypes.h"
#include <iostream>

struct rhs_van {
	Double eps;
	static const Int dim = 2;
	rhs_van(Double epss) : eps(epss) {}
	void operator() (const Double x, DoubleVec &y, DoubleVec &dydx) {
		dydx[0] = y[1];
		dydx[1] = ((1.0-y[0]*y[0])*y[1]-y[0])/eps;
	}
};

struct rhs_harmonic {
	Double omega;
	static const Int dim = 2;
	rhs_harmonic(Double _omega) : omega(_omega) {}
	void operator() (const Double x, DoubleVec &y, DoubleVec &dydx) {
		dydx[0] = y[1];
		dydx[1] = -omega*omega*y[0];
	}
};

int main() {
	const Double atol = 1.0e-6, 
	rtol = atol, 
	h1 = 0.01, 
	hmin = 0.0, 
	x1 = 0.0, 
	x2 = M_PI;
	DoubleVec ystart(rhs_harmonic::dim);
	ystart[0] = 2.0;
	ystart[1] = 0.0;
	ODEdata out(20);
	rhs_harmonic d(2.0);
	ODEsolver<ODEstepperPD853<rhs_harmonic> > ode(h1,hmin);
	ode.SetTolerance(atol, rtol);
	ode.SetOutput(out);
	ode.Integrate(d, ystart, x1, x2);
	for (Int i = 0; i < out.Count(); ++i) {
		std::cout << out.xSave[i] << " " << out.ySave[0][i] << " " <<
			out.ySave[1][i] << std::endl;
	}
	return 0;
}