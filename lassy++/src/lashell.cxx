#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/lashell.h"
using namespace std;


Circle::Circle(double r = 1.0, string c = "red") {
		radius = r;
		color = c;
	}

	double Circle::getRadius() {  // Member function (Getter)
		return radius;
	}

	string Circle::getColor() {   // Member function (Getter)
		return color;
	}

	double Circle::getArea() {    // Member function
		return radius*radius*3.1416;
	}

