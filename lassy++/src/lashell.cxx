#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShell.h"
using namespace std;


LaShell::LaShell(const char* vtk_fn)
{

	// Read from file 
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New(); 
	reader->SetFileName(vtk_fn); 
	reader->Update(); 

	_mesh_3d = vtkSmartPointer<vtkPolyData>::New(); 
	_mesh_3d->DeepCopy(reader->GetOutput());

}

LaShell::LaShell()
{
	_mesh_3d = vtkSmartPointer<vtkPolyData>::New();
}

vector<double> LaShell::GetMeshVertexValues()
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


void LaShell::GetMesh3D(vtkPolyData* mesh_output) {  // Member function (Getter)
	mesh_output->DeepCopy(_mesh_3d);
}

void LaShell::ExportVTK(char* vtk_fn)
{
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetInputData(_mesh_3d);
	writer->SetFileName(vtk_fn);
	writer->Update();
}


void LaShell::ConvertMaskToMesh(LaImage *la_mask, double threshold)
{
	char* temp_vtk_fn = "mask.vtk"; 
	la_mask->ConvertToVTKImage(temp_vtk_fn);

	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");

	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}


	vtkSmartPointer<vtkStructuredPointsReader > reader = vtkSmartPointer<vtkStructuredPointsReader >::New();
	reader->SetFileName(temp_vtk_fn);
	reader->Update();

	vtkSmartPointer<vtkStructuredPoints> temp_poly = vtkSmartPointer<vtkStructuredPoints>::New();

	temp_poly = reader->GetOutput(); 

	vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputData(temp_poly);
	surface->SetValue(0, threshold);

	vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
	vtkSmartPointer<vtkDecimatePro> deci = vtkSmartPointer<vtkDecimatePro>::New();

	smoother->SetNumberOfIterations(10);
	smoother->SetInputConnection(surface->GetOutputPort()); 
	smoother->Update();
	
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->ComputeCellNormalsOn();
	normals->SetInputConnection(smoother->GetOutputPort());
	//normals->FlipNormalsOn(); 
	//vtkSmartPointer<vtkPolyDataMapper> surfaceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	normals->Update();
	//surfaceMapper->SetInputConnection(normals->GetOutputPort());
	
	_mesh_3d = normals->GetOutput(); 
}


