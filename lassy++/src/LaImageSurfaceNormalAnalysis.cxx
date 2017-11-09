#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaImageSurfaceNormalAnalysis.h"

using namespace std;


LaImageSurfaceNormalAnalysis::LaImageSurfaceNormalAnalysis()
{
	_la_shell = new LaShell(); 
	_mesh_3d = vtkSmartPointer<vtkPolyData>::New(); 
	_vtk_logging = false; 
	_mask_image = NULL;
}

LaImageSurfaceNormalAnalysis::~LaImageSurfaceNormalAnalysis() {

}

void LaImageSurfaceNormalAnalysis::SetInputDataShell(LaShell* shell)
{
	_la_shell = shell; 

}

void LaImageSurfaceNormalAnalysis::SetInputDataBinary(LaImage* binary_img) {
	_la_binary = binary_img;
	
}

void LaImageSurfaceNormalAnalysis::SetInputDataImage(LaImage* intensity_img) {
	_la_image = intensity_img;
}

void LaImageSurfaceNormalAnalysis::SetInputDataImageMask(LaImage* mask_img) {
	_mask_image = mask_img;
}


LaShell* LaImageSurfaceNormalAnalysis::GetOutput() {
	LaShell* output = new LaShell();
	output->SetMesh3D(_mesh_3d); 
	return output; 
}


void LaImageSurfaceNormalAnalysis::SurfaceProjection(bool doLogging)
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

	// clear intensity log file 
	std::ofstream ofs;
	if (doLogging) {
		ofs.open("intensity_log.csv", std::ofstream::out | std::ofstream::trunc);
		ofs << "CellID,Normal index,Normal_Vec_X,Normal_Vec_Y,Normal_Vec_Z,Image intensity" << endl;
	}
	ofs.close();


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
			_la_image->WorldToImage(cP[0], cP[1], cP[2]);
			cX += cP[0]; cY += cP[1]; cZ += cP[2];
			num_points++;
		}

		cX /= num_points; cY /= num_points; cZ /= num_points;

		//		scar_image->WorldToImage(cX, cY, cZ); 
		_la_image->WorldToImage(pN[0], pN[1], pN[2]);


		//getIntensityAlongNormal(pN[0], pN[1], pN[2], cX, cY, cZ, normal_band, scalar);
		double scalar = 0, mean = 0, var = 1;
		_la_image->InterrogateImage(pN[0], pN[1], pN[2], cX, cY, cZ, scalar, doLogging, i, _mask_image);
		//scalar = 0;


		double sdev, sratio, sintensity;

		if (scalar > max_scalar)
			max_scalar = scalar;
		if (scalar < min_scalar)
			min_scalar = scalar;

		sdev = (scalar - mean) / sqrt(var);
		
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


void LaImageSurfaceNormalAnalysis::SetVTKLogging()
{
	_vtk_logging = true; 
}

void LaImageSurfaceNormalAnalysis::Update() {

	if (_la_binary != NULL) {
		_la_shell->ConvertMaskToMesh(_la_binary, 0.5);
	}
	
	cout << "\nConverted mask to shell, now performing surface analysis  .. " << endl;
	_la_shell->GetMesh3D(_mesh_3d);
	SurfaceProjection(_vtk_logging);


}