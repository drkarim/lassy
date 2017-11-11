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
	_step_size = 4; 
	_normal_step_shell = NULL;

	_normal_interrogate_algorithm = new LaImageNormalInterrogator();

	
}

LaImageSurfaceNormalAnalysis::~LaImageSurfaceNormalAnalysis() {

}

void LaImageSurfaceNormalAnalysis::SetStepSize(double steps)
{
	_step_size = steps; 
}

void LaImageSurfaceNormalAnalysis::SetInputDataShell(LaShell* shell)
{
	_la_shell = shell; 

}

void LaImageSurfaceNormalAnalysis::SetInputNormalStepShell(LaShell* shell)
{
	_normal_step_shell = shell; 
}

void LaImageSurfaceNormalAnalysis::SetInputDataBinary(LaImage* binary_img) {
	_la_binary = binary_img;
	
}

void LaImageSurfaceNormalAnalysis::SetInputDataImage(LaImage* intensity_img) {
	_la_image = intensity_img;
	_normal_interrogate_algorithm->SetInputData(intensity_img);
	//_normal_interrogate_algorithm->SetRecordLogs();
}

void LaImageSurfaceNormalAnalysis::SetInputDataImageMask(LaImage* mask_img) {
	_mask_image = mask_img;
	_normal_interrogate_algorithm->SetInputData2(mask_img);
}


LaShell* LaImageSurfaceNormalAnalysis::GetOutput() {
	LaShell* output = new LaShell();
	output->SetMesh3D(_mesh_3d); 
	return output; 
}


void LaImageSurfaceNormalAnalysis::SurfaceProjectionOnPoints(bool doLogging)
{

	double pN[3];
	double cP[3];
	double cX = 0, cY = 0, cZ = 0, max_scalar = -1, min_scalar = 1E10;
	int num_points = 0;
	
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
		ofs << "PointID,Normal index,Normal_Vec_X,Normal_Vec_Y,Normal_Vec_Z,Image intensity" << endl;
	}
	ofs.close();

	vtkSmartPointer<vtkFloatArray> pointNormals = vtkFloatArray::SafeDownCast(_mesh_3d->GetPointData()->GetNormals());

	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();		// the scalar at each polygon 
	

	vtkSmartPointer<vtkPolyData> normal_steps_polydata = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkFloatArray> normal_steps_polydata_scalars = vtkSmartPointer<vtkFloatArray>::New();

	if (_normal_step_shell != NULL)
	{
		_normal_step_shell->GetMesh3D(normal_steps_polydata);
		normal_steps_polydata_scalars = vtkFloatArray::SafeDownCast(normal_steps_polydata->GetPointData()->GetScalars()); 
	}

	

	for (int i = 0; i<_mesh_3d->GetNumberOfPoints(); i++)			// running through each polygon 
	{
		pointNormals->GetTuple(i, pN);
		_mesh_3d->GetPoint(i, cP); 


		_la_image->WorldToImage(pN[0], pN[1], pN[2]);
		_la_image->WorldToImage(cP[0], cP[1], cP[2]);

		double scalar;
		int which_direction[2]; 
		which_direction[0] = -1; 
		which_direction[1] = 1; 

		if (_normal_step_shell != NULL)
		{
			_step_size = normal_steps_polydata_scalars->GetTuple1(i); 
			which_direction[0] = 0; 
			which_direction[1] = 1; 
		}

		//_la_image->InterrogateImage(pN[0], pN[1], pN[2], cP[0], cP[1], cP[2], scalar, _step_size, which_direction, doLogging, i, _mask_image);
		_normal_interrogate_algorithm->SetLineOrigin(cP);
		_normal_interrogate_algorithm->SetDirectionVector(pN);
		_normal_interrogate_algorithm->SetInterrogationDirections(which_direction);
		_normal_interrogate_algorithm->Update(); 

		scalar = _normal_interrogate_algorithm->GetIntensity();

		if (scalar > max_scalar) {

			max_scalar = scalar;
		}

		if (scalar < min_scalar) {

			min_scalar = scalar;
		}
		
		if (scalar <= 0) {
			scalar = 0;
		}

		scalars->InsertTuple1(i, scalar);

	}	// end for 
	_mesh_3d->GetPointData()->SetScalars(scalars);
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
	
	
	vtkSmartPointer<vtkPolyData> normal_steps_polydata = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkFloatArray> normal_steps_polydata_scalars = vtkSmartPointer<vtkFloatArray>::New();


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

		double* cXYZ = new double[3]; 
		cXYZ[0] = cX; 
		cXYZ[1] = cY;
		cXYZ[2] = cZ;


		//getIntensityAlongNormal(pN[0], pN[1], pN[2], cX, cY, cZ, normal_band, scalar);
		double scalar = 0, mean = 0, var = 1;
		int* directions = new int[2]; 
		directions[0] = -1; 
		directions[1] = 1;
		//_la_image->InterrogateImage(pN[0], pN[1], pN[2], cX, cY, cZ, scalar, _step_size, directions, doLogging, i, _mask_image);

		_normal_interrogate_algorithm->SetLineOrigin(cXYZ);
		_normal_interrogate_algorithm->SetDirectionVector(pN);
		_normal_interrogate_algorithm->SetInterrogationDirections(directions);
		_normal_interrogate_algorithm->Update();
		

		scalar = _normal_interrogate_algorithm->GetIntensity();
		
		if (scalar > max_scalar)
			max_scalar = scalar;
		if (scalar < min_scalar)
			min_scalar = scalar;

																	//double scalarToPlot = scalar/mean; 
		if (scalar <= 0) scalar = 0;

		scalars->InsertTuple1(i, scalar);

	}	// end for 
	_mesh_3d->GetCellData()->SetScalars(scalars);
}


void LaImageSurfaceNormalAnalysis::SetVTKLogging()
{
	_vtk_logging = true; 
}

void LaImageSurfaceNormalAnalysis::Update() {

	if (_la_binary != NULL) {
		_la_shell->BinaryImageToShell(_la_binary, 0.5);
	}
	
	cout << "\nConverted mask to shell, now performing surface analysis  .. " << endl;
	_la_shell->GetMesh3D(_mesh_3d);
	//SurfaceProjection(_vtk_logging);
	SurfaceProjectionOnPoints(_vtk_logging);


}

void LaImageSurfaceNormalAnalysis::SetAggregationMethodToMax()
{
	_normal_interrogate_algorithm->SetAggregationMethodToMax();
}

void LaImageSurfaceNormalAnalysis::SetAggregationMethodToMean()
{
	_normal_interrogate_algorithm->SetAggregationMethodToMean();
}

void LaImageSurfaceNormalAnalysis::SetAggregationMethodToMedian()
{
	_normal_interrogate_algorithm->SetAggregationMethodToMedian();
}

void LaImageSurfaceNormalAnalysis::SetAggregationMethodToIntegral()
{
	_normal_interrogate_algorithm->SetAggregationMethodToIntegral();
}

void LaImageSurfaceNormalAnalysis::SetZScoreMean(double mean)
{
	_normal_interrogate_algorithm->SetZScoreMean(mean); 
}

void LaImageSurfaceNormalAnalysis::SetZScoreStd(double std)
{
	_normal_interrogate_algorithm->SetZScoreStd(std);
}
