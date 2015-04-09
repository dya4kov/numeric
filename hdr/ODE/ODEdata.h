#pragma once
#include "../numericTypes.h"

class ODEdata {
public:
	ODEdata(){ // no output
		kMax = -1;
		dense = false;
		count = 0;
	}
	ODEdata(const Int _nSave) {
		kMax = _nSave > 0 ? _nSave + 1 : 500;
		nSave = _nSave;
		count = 0;
		dense = nSave > 0 ? true : false;
	}
	void Init(const Int _dim, const Double _xFrom, const Double _xTo) {
		dim = _dim;
		if (kMax == -1) return;
		xSave = DoubleVec(0.0, kMax);
		ySave = DoubleMat(0.0, dim, kMax);
		xFrom = _xFrom;
		xTo = _xTo;
		if (dense) {
			xOut = xFrom;
			dxOut = (xTo - xFrom)/nSave;
		}
		count = 0;
	}
	void Resize() {
		kMax = kMax*2;
		xSave.resize(kMax);
		ySave.resize(dim, kMax);
	}
	bool IsDense() {
		return dense;
	}
	template <class Stepper>
	void SaveDense(Stepper &s, const Double xOut, const Double h) {
		if (count == kMax) Resize();
		for (Int i = 0; i < dim; ++i) {
			ySave[i][count] = s.DenseOut(i, xOut, h);
		}
		xSave[count++] = xOut;
	}
	void Save(const Double x, DoubleVec &y) {
		if (kMax <= 0) return;
		if (count == kMax) Resize();
		for (Int i = 0; i < dim; ++i) {
			ySave[i][count] = y[i];
		}
		xSave[count++] = x;
	}
	template <class Stepper>
	void Out(const Int nStep, const Double x, DoubleVec &y, Stepper s, const Double h) {
		if (!dense) {
			throw("dense output not set in Output!");
		}
		if (nStep == -1) {
			Save(x,y);
			xOut += dxOut;
		} 
		else {
			while ((x-xOut)*(xTo-xFrom) > 0.0) {
				SaveDense(s,xOut,h);
				xOut += dxOut;
			}
		}
	}
	Int Capacity() {
		return kMax;
	}
	Int Count() {
		return count;
	}
	void Reset() {
		xSave *= 0.0;
		ySave *= 0.0;
		count = 0;
	}

	DoubleVec xSave;
	DoubleMat ySave;

private:
	Int kMax; // Current capacity of storage arrays
	Int dim;
	Int nSave; // Number of intervals to save at for dense output
	bool dense; // true if dense output requested
	Int count; // Number of values actually saved
	Double xFrom, xTo, xOut, dxOut;
};
