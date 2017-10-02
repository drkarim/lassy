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


void LaShell::SurfaceProjection(LaImage* intensity_img, LaImage* mask_img)
{
	double pN[3]; 
	double cP[3];
	double cX = 0, cY = 0, cZ = 0, max_scalar = -1, min_scalar = 1E10;
	int num_points = 0; 
	vtkIdType num_cell_points;

	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	vtkSmartPointer<vtkIdList> cell_points = vtkSmartPointer<vtkIdList>::New();

	vtkSmartPointer<vtkFloatArray> cellNormals = vtkFloatArray::SafeDownCast(_mesh_3d->GetCellData()->GetNormals());

	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();		// the scalar at each polygon 
	vtkSmartPointer<vtkFloatArray> scalars_onlystdev = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkFloatArray> scalars_onlymultiplier = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkFloatArray> scalars_onlyintensity = vtkSmartPointer<vtkFloatArray>::New();
	
	for (int i = 0; i<_mesh_3d->GetNumberOfCells(); i++)			// running through each polygon 
	{
		cellNormals->GetTuple(i, pN);

		cX = 0; cY = 0; cZ = 0; num_points = 0;
		vtkIdType neighbor_point;
		_mesh_3d->GetCellPoints(i, cell_points);
		num_cell_points = cell_points->GetNumberOfIds();
	
		for (neighbor_point = 0; neighbor_point < num_cell_points; ++neighbor_point)
		{
			// Get the neighbor point id.           
			vtkIdType neighbor_point_id = cell_points->GetId(neighbor_point);

			// Get the neighbor point position. 
			//double cP[3]; 
			_mesh_3d->GetPoint(neighbor_point_id, cP);
			intensity_img->WorldToImage(cP[0], cP[1], cP[2]);
			cX += cP[0]; cY += cP[1]; cZ += cP[2];
			num_points++;
		}

		cX /= num_points; cY /= num_points; cZ /= num_points;
		
		//		scar_image->WorldToImage(cX, cY, cZ); 
		intensity_img->WorldToImage(pN[0], pN[1], pN[2]);

		
		//getIntensityAlongNormal(pN[0], pN[1], pN[2], cX, cY, cZ, normal_band, scalar);
		double scalar = 0, mean = 0, var = 1;
		intensity_img->InterrogateImage(pN[0], pN[1], pN[2], cX, cY, cZ, scalar, mask_img);
		//scalar = 0;
		

		double sdev, sratio, sintensity;

		if (scalar > max_scalar)
			max_scalar = scalar;
		if (scalar < min_scalar)
			min_scalar = scalar;

		sdev = (scalar - mean) / sqrt(var);
		sratio = scalar / mean;


		
		scalars_onlystdev->InsertTuple1(i, sdev);
		scalars_onlyintensity->InsertTuple1(i, scalar);
		scalars_onlymultiplier->InsertTuple1(i, sratio);

		// FOR DEFAULT scalar to plot 
		double scalarToPlot = (scalar - mean) / sqrt(var);			// the default what is plotted and displayed to end-user
																	//double scalarToPlot = scalar/mean; 

		if (scalarToPlot <= 0) scalarToPlot = 0;

		
		scalars->InsertTuple1(i, scalarToPlot);
		
	

	}	// end for 
	_mesh_3d->GetCellData()->SetScalars(scalars);
}

