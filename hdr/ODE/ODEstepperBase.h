#pragma once
#include "../numericTypes.h"

class ODEstepperBase {
public:
	ODEstepperBase(DoubleVec &_y, DoubleVec &_dydx, Double &_x);
	void SetTolerance(const Double _tolAbs, const Double _tolRel);
	void SetDenseOn();
	void SetDenseOff();
	Double LastStep() { return hDid; }
	Double NextStep() { return hNext; }
protected:
	Double &x;
	Double xOld;
	DoubleVec &y, &dydx;
	Double tolAbs, tolRel;
	bool dense;
	Double hDid;
	Double hNext;
	Int n, dim;
	DoubleVec yOut, yErr;
	Double EPS;
};

ODEstepperBase::ODEstepperBase(DoubleVec &_y, DoubleVec &_dydx, Double &_x) : x(_x), y(_y), dydx(_dydx) {
	dense = false;
	EPS = std::numeric_limits<Double>::epsilon();
}

void ODEstepperBase::SetTolerance(const Double _tolAbs, const Double _tolRel) {
	tolAbs = _tolAbs;
	tolRel = _tolRel;
}

void ODEstepperBase::SetDenseOn() { dense = true; }
void ODEstepperBase::SetDenseOff() { dense = false; }