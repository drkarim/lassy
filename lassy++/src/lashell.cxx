#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/lashell.h"
using namespace std;


LAtrium::LAtrium(const char* vtk_fn)
{

	// Read from file 
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New(); 
	reader->SetFileName(vtk_fn); 
	reader->Update(); 

	_mesh_3d = vtkSmartPointer<vtkPolyData>::New(); 
	_mesh_3d->DeepCopy(reader->GetOutput());

}

vector<double> LAtrium::GetMeshVertexValues()
{
	vtkSmartPointer<vtkFloatArray> scalar_array = vtkSmartPointer<vtkFloatArray>::New();
	

	if (_mesh_3d->GetPointData()->GetNumberOfArrays() > 0)
	{
		scalar_array = vtkFloatArray::SafeDownCast(_mesh_3d->GetPointData()->GetScalars());
		for (vtkIdType i = 0; i < _mesh_3d->GetNumberOfPoints(); ++i) {

			double this_scalar = scalar_array->GetTuple1(i);
			_mesh_vertex_values.push_back(this_scalar);
		}
	}
	return _mesh_vertex_values; 
}


void LAtrium::GetMesh3D(vtkPolyData* mesh_output) {  // Member function (Getter)
	mesh_output->DeepCopy(_mesh_3d);
}



