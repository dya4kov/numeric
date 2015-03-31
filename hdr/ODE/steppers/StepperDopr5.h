#pragma once
#include "../ODEstepperBase.h"
#include "../../numericTypes.h"
#include "../../mathUtils.h"

template<class RHStype>
class StepperDopr5 : public ODEstepperBase {
	// Dormand-Prince fifth-order Runge-Kutta step with monitoring of 
	// local truncation error to ensure accuracy and adjust stepsize.
public:
	typedef RHStype RHS;
	StepperDopr5(DoubleVec &_y, DoubleVec &_dydx, Double &_x);
	void Step(const Double hTry, RHStype &rhs);
	void dy(const Double h, RHStype &rhs);
	void PrepareDense(const Double h, RHStype &rhs);
	Double DenseOut(const Int i, const Double x, const Double h);
	Double Error();
	struct Controller {
		Double hNext,errOld;
		bool reject;
		Controller();
		bool Success(const Double err, Double &h);
	};
private:
	DoubleVec k2,k3,k4,k5,k6;
	DoubleVec rcont1,rcont2,rcont3,rcont4,rcont5;
	DoubleVec dydxnew;
	Controller con;
};

template<class RHStype>
StepperDopr5<RHStype>::StepperDopr5(DoubleVec &_y, DoubleVec &_dydx, Double &_x) : ODEstepperBase(_y, _dydx, _x) {
	dim = RHStype::dim;
	n = dim;
	yOut = DoubleVec(dim, 0.0);
	yErr = DoubleVec(dim, 0.0);
	k2 = DoubleVec(dim, 0.0);
	k3 = DoubleVec(dim, 0.0);
	k4 = DoubleVec(dim, 0.0);
	k5 = DoubleVec(dim, 0.0);
	k6 = DoubleVec(dim, 0.0);
	rcont1 = DoubleVec(dim, 0.0);
	rcont2 = DoubleVec(dim, 0.0);
	rcont3 = DoubleVec(dim, 0.0);
	rcont4 = DoubleVec(dim, 0.0);
	rcont5 = DoubleVec(dim, 0.0);
	dydxnew = DoubleVec(dim, 0.0);
}

template<class RHStype>
void StepperDopr5<RHStype>::Step(const Double hTry, RHStype &rhs) {
	Double h = hTry; // Set stepsize to the initial trial value
	for (;;) {
		dy(h, rhs); // Take a step
		Double err = Error(); // Evaluate accuracy
		if (con.Success(err, h)) break; 
		// Else step rejected. Try again with
        // reduced h set by controller
		if (abs(h) <= abs(x)*EPS) {
			throw("stepSize underflow in StepperDopr5");
		}
	}
	if (dense) PrepareDense(h,rhs);
	dydx = dydxnew; // Reuse last derivative evaluation for next step
	y = yOut;
	xOld = x; // Used for dense output 
	hDid = h;
	x += hDid;
	hNext = con.hNext;
}

template<class RHStype>
void StepperDopr5<RHStype>::dy(const Double h, RHStype &rhs) {
	static const Double
	c2 = 0.2, c3 = 0.3, c4 = 0.8, c5 = 8.0/9.0,
	a21 = 0.2,
	a31 = 3.0/40.0,        a32 = 9.0/40.0,
	a41 = 44.0/45.0,       a42 = -56.0/15.0,      a43 = 32.0/9.0,
	a51 = 19372.0/6561.0,  a52 = -25360.0/2187.0, a53 = 64448.0/6561.0, a54 = -212.0/729.0,
	a61 = 9017.0/3168.0,   a62 = -355.0/33.0,     a63 = 46732.0/5247.0, a64 = 49.0/176.0,     a65 = -5103.0/18656.0,
	a71 = 35.0/384.0,      a73 = 500.0/1113.0,    a74 = 125.0/192.0,    a75 = -2187.0/6784.0, a76 = 11.0/84.0,
	e1 = 71.0/57600.0,      e3 = -71.0/16695.0,    e4 = 71.0/1920.0, 
	e5 = -17253.0/339200.0, e6 = 22.0/525.0,       e7 = -1.0/40.0;
	DoubleVec yTemp(dim, 0.0);
	
	Int i;
	for (i = 0; i < n; ++i) { // First step
		yTemp[i] = y[i] + h*a21*dydx[i];
	}
	rhs(x + c2*h, yTemp, k2); // Second step
	for (i = 0; i < n; ++i) {
		yTemp[i] = y[i] + h*(a31*dydx[i] + a32*k2[i]);
	}
	rhs(x + c3*h, yTemp, k3); // Third step
	for (i = 0; i < n; ++i) {
		yTemp[i] = y[i] + h*(a41*dydx[i] + a42*k2[i] + a43*k3[i]);
	}
	rhs(x + c4*h, yTemp, k4); // Fourth step
	for (i = 0; i < n; ++i) {
		yTemp[i] = y[i] + h*(a51*dydx[i] + a52*k2[i] + a53*k3[i] + a54*k4[i]);
	}
	rhs(x + c5*h, yTemp, k5); // Fifth step
	for (i = 0; i < n; ++i) {
		yTemp[i] = y[i] + h*(a61*dydx[i] + a62*k2[i] + a63*k3[i] + a64*k4[i] + a65*k5[i]);
	}
	Double xph = x + h;
	rhs(xph, yTemp, k6); // Sixth step
	for (i = 0; i < n; ++i) {
		// Accumulate increments with proper weights
		yOut[i] = y[i] + h*(a71*dydx[i] + a73*k3[i] + a74*k4[i] + a75*k5[i] + a76*k6[i]);
	}
	rhs(xph,yOut,dydxnew); // Will also be first evaluation for next step.
	//Estimate error as difference between fourth- and fifth-order methods.
	for (i = 0; i < n; ++i) {
		yErr[i]=h*(e1*dydx[i] + e3*k3[i] + e4*k4[i] + e5*k5[i] + e6*k6[i] + e7*dydxnew[i]);
	}
}

template<class RHStype>
void StepperDopr5<RHStype>::PrepareDense(const Double h, RHStype &rhs) {
	// Store coefficients of interpolating polynomial for dense output in rcont1...rcont5.
	DoubleVec yTemp(dim, 0.0);
	static const Double
	d1 = -12715105075.0/11282082432.0,
	d3 = 87487479700.0/32700410799.0, 
	d4 = -10690763975.0/1880347072.0,
	d5 = 701980252875.0/199316789632.0, 
	d6 = -1453857185.0/822651844.0,
	d7 = 69997945.0/29380423.0;
	Double ydiff = 0.0;
	Double bspl = 0.0;
	for (Int i = 0; i < dim; ++i) {
		rcont1[i] = y[i];
		ydiff = yOut[i] - y[i];
		rcont2[i] = ydiff;
		bspl = h*dydx[i] - ydiff;
		rcont3[i] = bspl;
		rcont4[i] = ydiff - h*dydxnew[i] - bspl;
		rcont5[i] = h*(d1*dydx[i] + d3*k3[i] + d4*k4[i] + 
					   d5*k5[i] + d6*k6[i] + d7*dydxnew[i]);
	}
}

template<class RHStype>
Double StepperDopr5<RHStype>::DenseOut(const Int i, const Double x, const Double h) {
	// Evaluate interpolating polynomial for y[i] at location x, 
	// where xold <= x <= xold + h
	Double s = (x - xOld)/h;
	Double s1 = 1.0 - s;
	return rcont1[i] + s*(rcont2[i] + s1*(rcont3[i] + s*(rcont4[i] + s1*rcont5[i])));
}

template<class RHStype>
Double StepperDopr5<RHStype>::Error() {
	// Use yerr to compute norm of scaled error estimate.
	// A value less than one means the step wassuccessful
	Double err = 0.0;
	Double sk;
	for (Int i = 0; i < dim; ++i) {
		sk = tolAbs + tolRel*max(abs(y[i]),abs(yOut[i]));
		err += sqr(yErr[i]/sk);
	}
	return sqrt(err/dim);
}

template<class RHStype>
StepperDopr5<RHStype>::Controller::Controller() : reject(false), errOld(1.0e-4) {}

template <class RHStype>
bool StepperDopr5<RHStype>::Controller::Success(const Double err, Double &h) {
	// Returns true if err <= 1, false otherwise. If step was successful, 
	// sets hNext to the estimated optimal stepsize for the next step.
	// If the step failed, reduces h appropriately for another try.
	static const Double 
	beta = 0.0, 
	alpha = 0.2 - beta*0.75, 
	safe = 0.9, 
	minscale = 0.2,
	maxscale = 10.0;
	// Set beta to a nonzero value for PI control. beta = 0.04-0.08 is a good default.
	Double scale;
	if (err <= 1.0) { // Step succeeded. Compute hNext.
		if (err == 0.0) {
			scale = maxscale;
		}
		else { // PI control if beta != 0.
			scale = safe*pow(err, -alpha)*pow(errOld, beta);
			// Ensure minscale <= hNext/h <= maxscale.
			if (scale < minscale) scale = minscale;  
			if (scale > maxscale) scale = maxscale;
		}
		// Don’t let step increase if last one was rejected
		if (reject) {
			hNext = h*min(scale,1.0);
		}
		else {
			hNext = h*scale;
		}
		errOld = max(err, 1.0e-4); // Bookkeeping for next call
		reject = false;
		return true;
	}
	else {
		scale = max(safe*pow(err, -alpha), minscale);
		h *= scale;
		reject = true;
		return false;
	}
}