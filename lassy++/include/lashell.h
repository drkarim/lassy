#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#define MAXDIST 50.0
#include <string>

using namespace std; 


class Circle {
private:
	double radius;      // Data member (Variable)
	string color;       // Data member (Variable)

public:
	// Constructor with default values for data members
	Circle(double r, string c); 

	double getRadius(); 

	string getColor(); 

	double getArea();
}; 