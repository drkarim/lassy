#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataReader.h>
#define MAXDIST 50.0

#include "lashell.h"

int main(int argc, char * argv[])
{
	// Construct a Circle instance
	Circle c1(1.2, "blue");
	cout << "Radius=" << c1.getRadius() << " Area=" << c1.getArea()
		<< " Color=" << c1.getColor() << endl;

	
	return 0;
}
