#pragma once
#include "../numericTypes.h"
#include "../mathUtils.h"
#include "ODEdata.h"
#include <iostream>

template<class Stepper>
class ODEsolver {
public:
	ODEsolver(Double _hStart = 1e-6, Double _hMin = 0, Int _maxStep = 50000) : 
	dim(Stepper::RHS::dim), 
	y(0.0, dim),
	dydx(0.0, dim),
	x(0.0),
	s(y, dydx, x)
	{
		hStart = _hStart;
		hMin = _hMin;
		maxStep = _maxStep;
		nOk = 0;
		nBad = 0;
		EPS = std::numeric_limits<Double>::epsilon();
		dense = false;
	}
	void SetDenseOn();
	void SetDenseOff();
	void SetTolerance(const Double _tolAbs, const Double _tolRel);
	void SetStep(const Double _hStart);
	void SetOutput(ODEdata &_out);
	void Integrate(typename Stepper::RHS &rhs,
				   DoubleVec &_yStart, 
		 		   const Double _xFrom, const Double _xTo);
private:
	bool dense;
	Int maxStep;
	Int nOk;
	Int nBad;
	Int nStep;
	Int dim;
	Stepper s;
	Double tolAbs, tolRel;
	Double xFrom, xTo, x;
	Double hStart, hMin, h;
	Double EPS;
	DoubleVec y, dydx;
	DoubleVec yStart;
	ODEdata* out;
};

template<class Stepper>
void ODEsolver<Stepper>::SetDenseOn() { 
	dense = true;
	s.SetDenseOn();
}

template<class Stepper>
void ODEsolver<Stepper>::SetDenseOff() { 
	dense = false; 
	s.SetDenseOff();	
}

template<class Stepper>
void ODEsolver<Stepper>::SetTolerance(const Double _tolAbs, const Double _tolRel) {
	tolAbs = _tolAbs;
	tolRel = _tolRel;
	s.SetTolerance(tolAbs, tolRel);
}

template<class Stepper>
void ODEsolver<Stepper>::SetStep(const Double _hStart) {
	hStart = _hStart;
}

template<class Stepper>
void ODEsolver<Stepper>::SetOutput(ODEdata &_out) {
	out = &_out;
	if (out->IsDense()) SetDenseOn();
	else SetDenseOff();	
}

template<class Stepper>
void ODEsolver<Stepper>::Integrate(typename Stepper::RHS &rhs, DoubleVec &_yStart, 
		 				   const Double _xFrom, const Double _xTo) {
	yStart = _yStart;
	y = yStart;
	xFrom = _xFrom;
	xTo = _xTo;
	x = xFrom;
	out->Init(dim, xFrom, xTo);
	h = abs(hStart)*sign(xTo - xFrom);
	rhs(x, y, dydx); // store initial values
	if (dense) out->Out(-1, x, y, s, h);
	else out->Save(x,y);
	for (nStep = 0; nStep < maxStep; ++nStep) {
		if ((x + h*1.0001 - xTo)*(xTo - xFrom) > 0.0) {
			h = xTo - x; // if stepsize can overshoot, decrease
		}
		s.Step(h, rhs); // take a step
		if (s.LastStep() == h) ++nOk; else ++nBad;
		if (dense) {
			out->Out(nStep,x,y,s,s.LastStep());
		}
		else {
			out->Save(x,y);
		}
		// std::cout << "x = " << x << ", y[0] = " << y[0] << ", y[1] = " << y[1] <<
		// ", y[2] = " << y[2] << ", y[3] = " << y[3] << std::endl;
		if ((x - xTo)*(xTo - xFrom) >= 0.0) { // Are we done?
			_yStart = y; // Update ystart (??)
			// make sure last step gets saved
			if (out->Capacity() > 0 && abs(out->xSave[out->Count() - 1] - xTo) > 100.0*abs(xTo)*EPS)
				out->Save(x,y);
			return; // normal exit
		}
		if (abs(s.NextStep()) <= hMin) std::cerr << "Step size too small in ODEsolver" << std::endl;
		h = s.NextStep();
	}
	std::cerr << "Too many steps in routine ODEsolver" << std::cerr;
}