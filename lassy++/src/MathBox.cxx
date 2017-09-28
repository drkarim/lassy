#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/MathBox.h"
using namespace std;



void MathBox::normalizeVector(double &a, double &b, double &c)
{
	double magn;
	magn = sqrt(pow(a, 2) + pow(b, 2) + pow(c, 2));
	a *= (1 / magn);
	b *= (1 / magn);
	c *= (1 / magn);
}
