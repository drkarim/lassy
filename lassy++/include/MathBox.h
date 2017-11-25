#define HAS_VTK 1
#pragma once

using namespace std; 


#include <math.h>
#include <vector>
#include <algorithm> 


class MathBox {
private:
	
public:
	static void normalizeVector(double &a, double &b, double &c);
	static double CalcMean(vector<double> d);
	static double CalcMedian(vector<double> d);
	static double CalcStd(vector<double> scores, double mean);
}; 


struct Point3 {
	double _x, _y, _z;
	Point3(double x, double y, double z) : _x(x), _y(y), _z(z) {}

	// operator overload 
	bool operator<(Point3 const& other) {
		return _x < other._x;
	}
};
