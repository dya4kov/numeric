#pragma once
#include "../numericTypes.h"
#include "../mathUtils.h"

/**
* Fermi-Dirac function approximations are taken from:
* H. M. Antia, "Rational function approximations for Fermi-Dirac Integrals",
* The Astrophysical Journal Supplement Series, 84:101-108, 1993
* The relative accuracy is up to the 1e-12 
*/

template<class Func>
class FermiDirac {
	Func func;
public:
	FermiDirac() {}
	Double operator() (const Double x) {
		return func.value(x);
	}
};

struct DMhalf {
	Double value(const Double x) {
		static const Int m1 = 8;
		static const Int k1 = 8;
		static const Int m2 = 12;
		static const Int k2 = 12;
		static const Double a[8] = {1.71446374704454E+7,
									 3.88148302324068E+7,
									 3.16743385304962E+7,
									 1.14587609192151E+7,
									 1.83696370756153E+6,
									 1.14980998186874E+5,
									 1.98276889924768E+3,
									 1.00000000000000E+0};
		static const Double b[8] = {9.67282587452899E+6,
		 							 2.87386436731785E+7,
		 							 3.26070130734158E+7,
		 							 1.77657027846367E+7,
		 							 4.81648022267831E+6,
		 							 6.13709569333207E+5,
		 							 3.13595854332114E+4,
		 							 4.35061725080755E+2};
		static const Double c[12] = {-4.46620341924942E-15,
									 -1.58654991146236E-12,
									 -4.44467627042232E-10,
									 -6.84738791621745E-8,
									 -6.64932238528105E-6,
									 -3.69976170193942E-4,
									 -1.12295393687006E-2,
									 -1.60926102124442E-1,
									 -8.52408612877447E-1,
									 -7.45519953763928E-1,
									  2.98435207466372E+0,
									  1.00000000000000E+0};								  
		static const Double d[12] =	{-2.23310170962369E-15,
		                             -7.94193282071464E-13,
		                             -2.22564376956228E-10,
		                             -3.43299431079845E-8,
		                             -3.33919612678907E-6,
		                             -1.86432212187088E-4,
		                             -5.69764436880529E-3,
		                             -8.34904593067194E-2,
		                             -4.78770844009440E-1,
		                             -4.99759250374148E-1,
		                              1.86795964993052E+0,
		                              4.16485970495288E-1};
		static Double up;
		static Double down;
		static Double Dup;
		static Double Ddown;
		static Int i;
		static Double xpow[12] = {0};
		static Double t;
		up = 0;
		down = 0;
		Dup = 0;
		Ddown = 0;
		t = 0;
		xpow[0] = 1.0;
		if (x < 2.0) {
			t = exp(x);
			for (i = 1; i < m1; ++i) {
				xpow[i] = xpow[i - 1]*t;
			}
			for (i = 0; i < m1; ++i) {
				up += xpow[i]*a[i];
				down += xpow[i]*b[i];
			}
			for (i = 1; i < m1; ++i) {
				Dup += i*xpow[i - 1]*a[i];
				Ddown += i*xpow[i - 1]*b[i];
			}
			return t*(up/down + Dup*t/down - up*Ddown*t/(down*down));
		}
		else {
			t = sqrt(x);
			for (i = 1; i < m2; ++i) {
				xpow[i] = xpow[i - 1]/(x*x);
			}
			for (i = 0; i < m2; ++i) {
				up += xpow[i]*c[i];
				down += xpow[i]*d[i];
			}
			for (i = 1; i < m2; ++i) {
				Dup += i*xpow[i - 1]*c[i];
				Ddown += i*xpow[i - 1]*d[i];
			}
			return 0.5*up/down/t - 2.0/(x*x*t)*(Dup/down - up*Ddown/(down*down));
		}
	}		                              
};

struct Mhalf {
	Double value(const Double x) {
		// use maximum precision
		static const Int m1 = 8;
		static const Int k1 = 8;
		static const Int m2 = 12;
		static const Int k2 = 12;
		// define coefficients
		static const Double a[8] = {1.71446374704454E+7,
									 3.88148302324068E+7,
									 3.16743385304962E+7,
									 1.14587609192151E+7,
									 1.83696370756153E+6,
									 1.14980998186874E+5,
									 1.98276889924768E+3,
									 1.00000000000000E+0};
		static const Double b[8] = {9.67282587452899E+6,
		 							 2.87386436731785E+7,
		 							 3.26070130734158E+7,
		 							 1.77657027846367E+7,
		 							 4.81648022267831E+6,
		 							 6.13709569333207E+5,
		 							 3.13595854332114E+4,
		 							 4.35061725080755E+2};
		static const Double c[12] = {-4.46620341924942E-15,
									 -1.58654991146236E-12,
									 -4.44467627042232E-10,
									 -6.84738791621745E-8,
									 -6.64932238528105E-6,
									 -3.69976170193942E-4,
									 -1.12295393687006E-2,
									 -1.60926102124442E-1,
									 -8.52408612877447E-1,
									 -7.45519953763928E-1,
									  2.98435207466372E+0,
									  1.00000000000000E+0};								  
		static const Double d[12] =	{-2.23310170962369E-15,
		                             -7.94193282071464E-13,
		                             -2.22564376956228E-10,
		                             -3.43299431079845E-8,
		                             -3.33919612678907E-6,
		                             -1.86432212187088E-4,
		                             -5.69764436880529E-3,
		                             -8.34904593067194E-2,
		                             -4.78770844009440E-1,
		                             -4.99759250374148E-1,
		                              1.86795964993052E+0,
		                              4.16485970495288E-1};
		static Double up;
		static Double down;
		static Int i;
		static Double xpow[12] = {0};
		static Double t;
		up = 0;
		down = 0;
		t = 0;
		xpow[0] = 1.0;
		if (x < 2.0) {
			t = exp(x);
			for (i = 1; i < k1; ++i) {
				xpow[i] = xpow[i - 1]*t;
			}
			for (i = 0; i < m1; ++i) {
				up += xpow[i]*a[i];
			}
			for (i = 0; i < k1; ++i) {
				down += xpow[i]*b[i];
			}	
		}
		else {
			t = sqrt(x);
			for (i = 1; i < k2; ++i) {
				xpow[i] = xpow[i - 1]/(x*x);
			}
			for (i = 0; i < m2; ++i) {
				up += xpow[i]*c[i];			
			}
			for (i = 0; i < k2; ++i) {
				down += xpow[i]*d[i];
			}
		}	
		return t*up/down;
	}
};

struct Half {
	Double value(const Double x) {
		// use maximum precision
		static const Int m1 = 8;
		static const Int k1 = 8;
		static const Int m2 = 11;
		static const Int k2 = 12;
		// define coefficients
		static const Double a[8] = {5.75834152995465E+6,
									1.30964880355883E+7,
									1.07608632249013E+7,
									3.93536421893014E+6,
									6.42493233715640E+5,
									4.16031909245777E+4,
									7.77238678539648E+2,
									1.00000000000000E+0};
		static const Double b[8] = {6.49759261942269E+6,
		 							1.70750501625775E+7,
		 							1.69288134856160E+7,
		 							7.95192647756086E+6,
		 							1.83167424554505E+6,
		 							1.95155948326832E+5,
		 							8.17922106644547E+3,
		 							9.02129136642157E+1};
		static const Double c[11] = {4.85378381173415E-14,
									 1.64429113030738E-11,
									 3.76794942277806E-9,
									 4.69233883900644E-7,
									 3.40679845803144E-5,
									 1.32212995937796E-3,
									 2.60768398973913E-2,
									 2.48653216266227E-1,
									 1.08037861921488E+0,
									 1.91247528779676E+0,
									 1.00000000000000E+0};								  
		static const Double d[12] =	{7.28067571760518E-14,
		                             2.45745452167585E-11,
		                             5.62152894375277E-9,
		                             6.96888634549649E-7,
		                             5.02360015186394E-5,
		                             1.92040136756592E-3,
		                             3.66887808002874E-2,
		                             3.24095226486468E-1,
		                             1.16434871200131E+0,
		                             1.34981244060549E+0,
		                             2.01311836975930E-1,
		                            -2.14562434782759E-2};
		static Double up;
		static Double down;
		static Int i;
		static Double xpow[12] = {0};
		static Double t;
		up = 0;
		down = 0;
		t = 0;
		xpow[0] = 1.0;
		if (x < 2.0) {
			t = exp(x);
			for (i = 1; i < k1; ++i) {
				xpow[i] = xpow[i - 1]*t;
			}
			for (i = 0; i < m1; ++i) {
				up += xpow[i]*a[i];
			}
			for (i = 0; i < k1; ++i) {
				down += xpow[i]*b[i];
			}	
		}
		else {
			t = sqrt(x);
			for (i = 1; i < k2; ++i) {
				xpow[i] = xpow[i - 1]/(x*x);
			}
			for (i = 0; i < m2; ++i) {
				up += xpow[i]*c[i];			
			}
			for (i = 0; i < k2; ++i) {
				down += xpow[i]*d[i];
			}
			t = t*x;
		}	
		return t*up/down;
	}
};

struct ThreeHalf {
	Double value(const Double x) {
		// use maximum precision
		static const Int m1 = 7;
		static const Int k1 = 8;
		static const Int m2 = 10;
		static const Int k2 = 11;
		// define coefficients
		static const Double a[7] = {4.32326386604283E+4,
									8.55472308218786E+4,
									5.95275291210962E+4,
									1.77294861572005E+4,
									2.21876607796460E+3,
									9.90562948053193E+1,
									1.00000000000000E+0};
		static const Double b[8] = {3.25218725353467E+4,
		 							7.01022511904373E+4,
		 							5.50859144223638E+4,
		 							1.95942074576400E+4,
		 							3.20803912586318E+3,
		 							2.20853967067789E+2,
		 							5.05580641737527E+0,
		 							1.99507945223266E-2};
		static const Double c[10] = {2.80452693148553E-13,
									 8.60096863656367E-11,
									 1.62974620742993E-8,
									 1.63598843752050E-6,
									 9.12915407846722E-5,
									 2.62988766922117E-3,
									 3.85682997219346E-2,
									 2.78383256609605E-1,
									 9.02250179334496E-1,
									 1.00000000000000E+0};								  
		static const Double d[11] =	{7.01131732871184E-13,
		                             2.10699282897576E-10,
		                             3.94452010378723E-8,
		                             3.84703231868724E-6,
		                             2.04569943213216E-4,
		                             5.31999109566385E-3,
		                             6.39899717779153E-2,
		                             3.14236143831882E-1,
		                             4.70252591891375E-1,
		                            -2.15540156936373E-2,
		                             2.34829436438087E-3};
		static Double up;
		static Double down;
		static Int i;
		static Double xpow[11] = {0};
		static Double t;
		up = 0;
		down = 0;
		t = 0;
		xpow[0] = 1.0;
		if (x < 2.0) {
			t = exp(x);
			for (i = 1; i < k1; ++i) {
				xpow[i] = xpow[i - 1]*t;
			}
			for (i = 0; i < m1; ++i) {
				up += xpow[i]*a[i];
			}
			for (i = 0; i < k1; ++i) {
				down += xpow[i]*b[i];
			}	
		}
		else {
			t = sqrt(x);
			for (i = 1; i < k2; ++i) {
				xpow[i] = xpow[i - 1]/(x*x);
			}
			for (i = 0; i < m2; ++i) {
				up += xpow[i]*c[i];			
			}
			for (i = 0; i < k2; ++i) {
				down += xpow[i]*d[i];
			}
			t = t*x*x;
		}	
		return t*up/down;
	}
};
