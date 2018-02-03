#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellEnclosureDistance.h"

using namespace std;


LaShellEnclosureDistance::LaShellEnclosureDistance()
{
	_output_shell = new LaShell();
	_which_direction = -1;		// Always -1 to measure enclosed distance using direction opposite to surface normal
	_do_logging = false;
}

LaShellEnclosureDistance::~LaShellEnclosureDistance() {
	delete _output_shell; 

}


void LaShellEnclosureDistance::SetInputData(LaShell* shell)
{
	_source_shell = shell;
}

void LaShellEnclosureDistance::SetLoggingToTrue()
{
	_do_logging = true;
}


LaShell* LaShellEnclosureDistance::GetOutput() {
	return _output_shell; 
}


void LaShellEnclosureDistance::MoveStartingPositionBy(double* start, double which_direction, double* direction_vec, double move_distance, double* new_start)
{

	new_start[0] = start[0] + (which_direction*move_distance*direction_vec[0]);
	new_start[1] = start[1] + (which_direction*move_distance*direction_vec[1]);
	new_start[2] = start[2] + (which_direction*move_distance*direction_vec[2]);

}



void LaShellEnclosureDistance::Update() {

	double pN[3], pStart[3], new_pStart[3], pEnd[3], tolerance = .001, max_dist = 10, move_distance =1;

	// Outputs of vtkModifiedBSPTree 
	double t; // Parametric coordinate of intersection (0 (corresponding to p1) to 1 (corresponding to p2))
	double x[3]; // The coordinate of the intersection
	double pcoords[3];
	int subId;

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	std::ofstream ofs;
	if (_do_logging) {
		ofs.open("Shell_Enclosure_Distance_Log.csv", std::ofstream::out | std::ofstream::trunc);

		ofs << "pointID,pX,pY,pZ,Line_Start_X,Line_Start_Y,Line_Start_Z,Line_End_X, Line_End_Y,Line_End_Z, intersect_X,intersect_Y,intersect_Z,distance" << endl;
		
	}

	vtkSmartPointer<vtkPolyData> ShellPolyData = vtkSmartPointer<vtkPolyData>::New();
	_source_shell->GetMesh3D(ShellPolyData);

	vtkSmartPointer<vtkPolyDataNormals> SourcePolyNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	SourcePolyNormals->ComputeCellNormalsOn();
	SourcePolyNormals->SetInputData(ShellPolyData);
	SourcePolyNormals->Update();

	vtkSmartPointer<vtkPolyData> OutputPoly = vtkSmartPointer<vtkPolyData>::New();
	OutputPoly->DeepCopy(ShellPolyData);

	vtkSmartPointer<vtkFloatArray> OutputPolyScalars = vtkSmartPointer<vtkFloatArray>::New();
	OutputPolyScalars->SetNumberOfComponents(1);


	vtkSmartPointer<vtkFloatArray> Source_pNormals = vtkFloatArray::SafeDownCast(ShellPolyData->GetPointData()->GetNormals());

	// See https://www.vtk.org/Wiki/VTK/Examples/Cxx/DataStructures/ModifiedBSPTree_IntersectWithLine
	// Create the tree
	vtkSmartPointer<vtkModifiedBSPTree> bspTree = vtkSmartPointer<vtkModifiedBSPTree>::New();
	bspTree->SetDataSet(ShellPolyData);
	bspTree->BuildLocator();


	for (vtkIdType i = 0; i < ShellPolyData->GetNumberOfPoints(); ++i) {

		ShellPolyData->GetPoint(i, pStart);
		
		Source_pNormals->GetTuple(i, pN);
		MoveStartingPositionBy(pStart, -1, pN, move_distance, new_pStart);

		
		LaShellShellIntersection::GetFiniteLine(new_pStart, pN, max_dist, _which_direction, pEnd);
		

		// find intersection with target, x is the new intersection
		vtkIdType iD = bspTree->IntersectWithLine(new_pStart, pEnd, tolerance, t, x, pcoords, subId);
		
		float distance_to_target = LaShellShellIntersection::GetEuclidean(pStart, x);

		

		if (_do_logging)
		{
			ofs << i << "," << pStart[0] << "," << pStart[1] << "," << pStart[2] << "," << new_pStart[0] << "," << new_pStart[1] << "," << new_pStart[2] << ","
				"" << pEnd[0] << "," << pEnd[1] << "," << pEnd[2] << ""
				"," << x[0] << "," << x[1] << "," << x[2] << "," << distance_to_target << endl;
 		}

		OutputPolyScalars->InsertNextTuple1(distance_to_target);

	} // end for 

	OutputPoly->GetPointData()->SetScalars(OutputPolyScalars);


	if (_do_logging)
	{
		ofs.close();
	}

	_output_shell->SetMesh3D(OutputPoly);
}
