#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShell.h"
#include "../include/ShellEntropy.h"
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


void LaShell::GetMesh3D(vtkSmartPointer<vtkPolyData> mesh_output) {  // Member function (Getter)
	mesh_output->DeepCopy(_mesh_3d);
}

void LaShell::SetMesh3D(vtkSmartPointer<vtkPolyData> input) {  // Member function (Getter)
	_mesh_3d->DeepCopy(input);
}

void LaShell::ExportVTK(char* vtk_fn)
{
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetInputData(_mesh_3d);
	writer->SetFileName(vtk_fn);
	writer->Update();
}

void LaShell::GetMinimumMaximum(double& min, double& max)
{
	// Not yet implemented 
	min=0; 
	max=3;
}

void LaShell::BinaryImageToShell(LaImage *la_mask, double threshold)
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

	smoother->SetNumberOfIterations(1000);
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




void LaShell::ConvertToPointData()
{
	vtkSmartPointer<vtkCellDataToPointData> cell_to_point = vtkSmartPointer<vtkCellDataToPointData>::New(); 
	cell_to_point->SetInputData(_mesh_3d); 
	cell_to_point->PassCellDataOn(); 
	cell_to_point->Update();
	_mesh_3d->DeepCopy(cell_to_point->GetPolyDataOutput()); 
}

void LaShell::ComputeMeshNeighbourhoodTransform(vtkSmartPointer<vtkPolyData> mesh_output)
{
	ShellEntropy* entropy = new ShellEntropy(_mesh_3d);
}