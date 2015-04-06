#pragma once
#include <vector>
#include <limits>
#include <valarray>

typedef double Double;
typedef int Int;
// typedef std::vector<double> DoubleVec;
// typedef std::vector<DoubleVec> DoubleMat;
typedef std::vector<int> IntVec;
typedef std::vector<IntVec> IntMat;

template<class T> 
class Slice_iter {
	std::valarray<T>* v;
	std::slice s;
	size_t curr;

	T& ref(size_t i) const { return (*v)[s.start() + i*s.stride()]; }
public:
	Slice_iter(std::valarray<T>* vv, std::slice ss) : v(vv), s(ss), curr(0) {}

	Slice_iter end() {
		Slice_iter t = *this;
		t.curr = s.size();
		return t;
	}

	size_t size() { return s.size(); }

	Slice_iter& operator++() { curr++; return *this; }
	Slice_iter& operator++(int) {Slice_iter t = *this; curr++; return t; } 

	T& operator[] (size_t i) { return ref(curr = i); }
	T& operator() (size_t i) { return ref(curr = i); }
	T& operator*() { return ref(curr); }
};

template<class T> bool operator== (const Slice_iter<T>& p, const Slice_iter<T>& q) {
		return p.curr == q.curr && 
			   p.s.stride() == q.s.stride() && 
			   p.s.start() == q.s.start();
}

template<class T> bool operator!= (const Slice_iter<T>& p, const Slice_iter<T>& q) {
	return !(p == q);
}

template<class T> bool operator< (const Slice_iter<T>& p, const Slice_iter<T>& q) {
	return p.curr < q.curr && 
		   p.s.stride() == q.s.stride() && 
		   p.s.start() == q.s.start();	
}

template<class T> 
class Cslice_iter {
	std::valarray<T>* v;
	std::slice s;
	size_t curr;

	const T& ref(size_t i) const { return (*v)[s.start() + i*s.stride()]; }
public:
	Cslice_iter(std::valarray<T>* vv, std::slice ss) : v(vv), s(ss), curr(0) {}

	Cslice_iter end() {
		Cslice_iter t = *this;
		t.curr = s.size();
		return t;
	}

	size_t size() { return s.size(); }

	Cslice_iter& operator++() { curr++; return *this; }
	Cslice_iter& operator++(int) {Cslice_iter t = *this; curr++; return t; } 

	const T& operator[] (size_t i) { return ref(curr = i); }
	const T& operator() (size_t i) { return ref(curr = i); }
	const T& operator*() { return ref(curr); }	
};

template<class T> bool operator== (const Cslice_iter<T>& p, const Cslice_iter<T>& q) {
	return p.curr == q.curr && 
		   p.s.stride() == q.s.stride() && 
		   p.s.start() == q.s.start();
}

template<class T> bool operator!= (const Cslice_iter<T>& p, const Cslice_iter<T>& q) {
	return !(p == q);
}

template<class T> bool operator< (const Cslice_iter<T>& p, const Cslice_iter<T>& q) {
	return p.curr < q.curr && 
		   p.s.stride() == q.s.stride() && 
		   p.s.start() == q.s.start();	
}

class Vector {
	std::valarray<Double>* v;
	size_t n;
public:
	Vector();
	Vector(size_t _size);
	Vector(const Double& value, size_t _size);
	Vector(const Vector&);
	Vector& operator=(const Vector&);
	~Vector();

	size_t size() const { return n; }

	Double& operator() (size_t i) { return (*v)[i]; }
	Double operator() (size_t i) const { return (*v)[i]; }
	Double& operator[] (size_t i) { return (*v)[i]; }
	Double operator[] (size_t i) const { return (*v)[i]; }

	Vector& operator*=(Double a) { (*v) *= a; return *this; }

	Vector& resize(size_t newSize);

	std::valarray<Double>& array() { return *v; }
};

Vector::Vector() {
	v = NULL;
	n = 0;
}

Vector::Vector(const Vector& _v) {
	n = _v.size();
	v = new std::valarray<Double>(n);
	*v = *(_v.v);
}

Vector& Vector::operator=(const Vector& _v) {
	if (this == &_v) {
		return *this;
	}
	if (n != _v.size()) {
		if (v != NULL) delete v;
		v = new std::valarray<Double>(_v.size());
	}
	n = _v.size();
	(*v) = *(_v.v);
	return *this;
}

Vector::Vector(size_t _size) {
	n = _size;
	v = new std::valarray<Double>(n);
}

Vector::Vector(const Double& value, size_t _size) {
	n = _size;
	v = new std::valarray<Double>(value, n);
}

Vector::~Vector() {
	delete v;
}

Vector& Vector::resize(size_t newSize) {
	std::valarray<Double>* vnew = new std::valarray<Double>(newSize);
	for (size_t i = 0; i < n; ++i) {
		(*vnew)[i] = (*v)[i];
	}
	delete v;
	v = vnew;
	n = newSize;
	return *this;
}

class Matrix { // From B. Straustrup "C++ programming language"
	std::valarray<Double>* v;
	size_t d1, d2;
public:

	Matrix();
	Matrix(size_t x, size_t y);
	Matrix(const Double& value, size_t x, size_t y);
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);
	~Matrix(); 

	size_t size() const { return d1*d2; }
	size_t nrows() const { return d1; }
	size_t ncols() const { return d2; }

	inline Slice_iter<Double> row(size_t i) {
		return Slice_iter<Double>(v, std::slice(i, d2, d1));
	}

	inline Cslice_iter<Double> row(size_t i) const {
		return Cslice_iter<Double>(v, std::slice(i, d2, d1));
	}

	inline Slice_iter<Double> column(size_t i) {
		return Slice_iter<Double>(v, std::slice(i*d2, d2, 1));
	}

	inline Cslice_iter<Double> column(size_t i) const {
		return Cslice_iter<Double>(v, std::slice(i*d2, d2, 1));
	}

	Double& operator() (size_t x, size_t y); // Fortran
	Double operator() (size_t x, size_t y) const;

	Slice_iter<Double> operator() (size_t i) { return row(i); }
	Cslice_iter<Double> operator() (size_t i) const { return row(i); }

	Slice_iter<Double> operator[] (size_t i) { return row(i); } // C
	Cslice_iter<Double> operator[] (size_t i) const { return row(i); }

	Matrix& operator*=(Double);

	Matrix& resize(size_t newX, size_t newY);

	std::valarray<Double>& array() { return *v; }
};

Matrix::Matrix() {
	v = NULL;
	d1 = 0;
	d2 = 0;
}

Matrix::Matrix(const Matrix& m) {
	d1 = m.nrows();
	d2 = m.ncols();
	v = new std::valarray<Double>(d1*d2);
	*v = *(m.v);
}

Matrix& Matrix::operator=(const Matrix& m) {
	if (this == &m) {
		return *this;
	}
	if (d1*d2 != m.size()) {
		if (v != NULL) delete v;
		v = new std::valarray<Double>(m.size());
	}
	*v = *(m.v);
	d1 = m.nrows();
	d2 = m.ncols();
	return *this;
}

Matrix::Matrix(size_t x, size_t y) {
	d1 = x;
	d2 = y;
	v = new std::valarray<Double>(x*y);
}

Matrix::Matrix(const Double& value, size_t x, size_t y) {
	d1 = x;
	d2 = y;
	v = new std::valarray<Double>(value, x*y);
}

Matrix::~Matrix() {
	delete v;
}

Double& Matrix::operator() (size_t i, size_t j) {
	return row(i)[j];
}

// Double mul(Cslice_iter<Double>& v1, const std::valarray<Double>& v2) {
// 	Double res = 0;
// 	size_t i = 0;
// 	for (size_t i; i < v1.size(); ++i) res += v1[i]*v2[i];
// }

// std::valarray<Double> operator* (const Matrix& m, const std::valarray<Double>& v) {
// 	std::valarray<Double> res(m.nrows());
// 	for (size_t i = 0; i < m.nrows(); ++i) res[i] = mul(m.row(i), v);
// 	return res;
// }

Matrix& Matrix::operator*= (Double d) {
	(*v) *= d;
	return *this;
}

Matrix& Matrix::resize(size_t newX, size_t newY) {
	std::valarray<Double>* vnew = new std::valarray<Double>(newX*newY);
	for (size_t i = 0; i < d1; ++i) {
		for (size_t j = 0; j < d2; ++j) {
			(*vnew)[i + j*newX] = (*v)[i + j*d1];
		}
	}
	delete v;
	v = vnew;
	d1 = newX;
	d2 = newY;
	return *this;
}

typedef Vector DoubleVec;
typedef Matrix DoubleMat;