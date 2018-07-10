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
	_shell_only_no_mapping = false;
	_mask_image = NULL;
	_step_size = 4;
	_normal_step_shell = NULL;
	_output_shell = new LaShell();

	_normal_interrogate_algorithm = new LaImageNormalInterrogator();
	


}

LaImageSurfaceNormalAnalysis::~LaImageSurfaceNormalAnalysis() {
	delete _la_shell;
	delete _normal_interrogate_algorithm;
	delete _output_shell;
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
	
	
	return _output_shell;
}

void LaImageSurfaceNormalAnalysis::SetOutputFileName(char* fn)
{
	_output_fn = fn;
}


void LaImageSurfaceNormalAnalysis::SurfaceProjectionOnPoints()
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
	/*std::ofstream ofs;
	ofs.open("intensity_log.csv", std::ofstream::out | std::ofstream::trunc);
	ofs << "Please note: Pixels on the normal are printed below, but the aggregate (mean, max, integral, etc.) is obtained from 3x3x3 neighbourhood around each pixel " << endl;
	ofs << "NormalStep,CentrePixel_X,CentrePixelY,CentrePixelZ,PixelValue" << endl;
	ofs.close();*/

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

		if (_normal_step_shell != NULL)
		{
			_step_size = normal_steps_polydata_scalars->GetTuple1(i);
			which_direction[0] = 0;
			which_direction[1] = 1;
		}
		else {
			which_direction[0] = -1;
			which_direction[1] = 1;

		}

		//_la_image->InterrogateImage(pN[0], pN[1], pN[2], cP[0], cP[1], cP[2], scalar, _step_size, which_direction, doLogging, i, _mask_image);
		_normal_interrogate_algorithm->SetLineOrigin(cP);
		_normal_interrogate_algorithm->SetDirectionVector(pN);
		_normal_interrogate_algorithm->SetInterrogationDirections(which_direction);
		_normal_interrogate_algorithm->SetStepSize(_step_size);
		_normal_interrogate_algorithm->SetLoggingLevel1ToTrue();
		// _normal_interrogate_algorithm->SetLoggingLevel2ToTrue();
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
	_output_shell->SetMesh3D(_mesh_3d);
	_output_shell->ExportVTK(_output_fn);
}




void LaImageSurfaceNormalAnalysis::SetVTKLogging()
{
	_vtk_logging = true;
}

void LaImageSurfaceNormalAnalysis::SetMethodToNoMapping() 
{
	_shell_only_no_mapping = true;
}

void LaImageSurfaceNormalAnalysis::Update() {

	if (_la_binary != NULL) {
		_la_shell->BinaryImageToShell(_la_binary, 0.5);
		_la_shell->GetMesh3D(_mesh_3d);
	}

	if  (!_shell_only_no_mapping) 
	{
		cout << "\nConverted mask to shell, now performing surface analysis  .. " << endl;
		

		//SurfaceProjection(_vtk_logging);
		SurfaceProjectionOnPoints();
	}

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
