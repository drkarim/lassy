#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellExtractArray.h"

using namespace std;


LaShellExtractArray::LaShellExtractArray()
{
	_source_la = new LaShell(); 
	_output_la = new LaShell(); 
}

LaShellExtractArray::~LaShellExtractArray() {}


void LaShellExtractArray::SetInputData(LaShell* shell) 
{
	_source_la = shell; 
}

void LaShellExtractArray::SetArrayName(const char* array_name)
{
	stringstream ss; 
	ss << array_name; 
	_array_name = ss.str(); 
}



LaShell* LaShellExtractArray::GetOutput() {
	return _output_la; 
}


void LaShellExtractArray::Update() {
	
	int n, k = -1;
	
	vtkSmartPointer<vtkPolyData> source_poly_data = vtkSmartPointer<vtkPolyData>::New(); 
	_source_la->GetMesh3D(source_poly_data); 


	vtkIdType numberOfPointArrays = source_poly_data->GetPointData()->GetNumberOfArrays();
	cout << "Number of arrays =  " << numberOfPointArrays << endl;

	for (vtkIdType i = 0; i < numberOfPointArrays; i++)
	{
		cout << "Array " << i << ", name = " << source_poly_data->GetPointData()->GetArray(i)->GetName() << endl;
		if (strcmp(source_poly_data->GetPointData()->GetArray(i)->GetName(), _array_name.c_str()) == 0) {
			cout << "Found array at location " << i << endl;
			k = i;
		}
	}

	if (k == -1)
		cout << "\n\nERROR: Could not find array, could be due to case-senstivity" << endl;
	else {

		vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
		scalars = vtkFloatArray::SafeDownCast(source_poly_data->GetPointData()->GetArray(k));


		vtkSmartPointer<vtkPolyData> OutputPoly = vtkSmartPointer<vtkPolyData>::New();
		OutputPoly->CopyStructure(source_poly_data);

		OutputPoly->GetPointData()->SetScalars(scalars);

	

		_output_la->SetMesh3D(OutputPoly);

	}

}
