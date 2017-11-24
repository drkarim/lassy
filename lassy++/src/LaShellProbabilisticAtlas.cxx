#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellProbabilisticAtlas.h"

using namespace std;


void LaShellProbabilisticAtlas::SetAtlasConstructionToUseDirectCopy()
{
	_which_method = USE_DIRECT_COPY; 
}

void LaShellProbabilisticAtlas::SetAtlasConstructionToUseClosestPoint()
{
	_which_method = USE_CLOSEST_POINT; 
}

void LaShellProbabilisticAtlas::ReadShellComputeDisplacement(string poly_data_fn)
{
	vtkSmartPointer<vtkPolyDataReader> reader1 = vtkSmartPointer<vtkPolyDataReader>::New();
	reader1->SetFileName(poly_data_fn.c_str());
	reader1->Update();

	cout << "Processing next file for displacment: " << poly_data_fn << " ... ";
	vtkSmartPointer<vtkPolyData> TargetPolyData = vtkSmartPointer<vtkPolyData>::New();
	TargetPolyData = reader1->GetOutput();

	vtkSmartPointer<vtkPointLocator> Target_Poly_PointLocator = vtkSmartPointer<vtkPointLocator>::New();
	Target_Poly_PointLocator->SetDataSet(TargetPolyData);
	Target_Poly_PointLocator->AutomaticOn();
	Target_Poly_PointLocator->BuildLocator();


	vtkSmartPointer<vtkFloatArray> Target_Scalars = vtkSmartPointer<vtkFloatArray>::New();
	Target_Scalars = vtkFloatArray::SafeDownCast(TargetPolyData->GetPointData()->GetScalars());


	_displacements.resize(_SourcePolyData->GetNumberOfPoints(), vector<double>(_total_targets));

	double source_vertex[3], target_vertex[3];
	
	for (vtkIdType i = 0; i < _SourcePolyData->GetNumberOfPoints(); ++i) {

		_SourcePolyData->GetPoint(i, source_vertex);
		
		double target_scalar_to_source; 

		if (_which_method == USE_CLOSEST_POINT)
		{
			vtkIdType id_on_target = Target_Poly_PointLocator->FindClosestPoint(source_vertex);
			TargetPolyData->GetPoint(id_on_target, target_vertex);
			target_scalar_to_source = GetEuclidean(source_vertex, target_vertex);

		}
		else if (_which_method == USE_DIRECT_COPY)
		{
			target_scalar_to_source = Target_Scalars->GetTuple1(i); 
		}

		
		if (i < _displacements.size() && _num_targets_read < _displacements[i].size())
		{
			_displacements[i][_num_targets_read] = target_scalar_to_source;
		}
		
	}

	_num_targets_read++; 

	cout << "completed!" << endl;
}





void LaShellProbabilisticAtlas::Update() {

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	if (_num_targets == MULTIPLE_TARGETS)
	{
		ReadShellNameList(_multiple_target_fn.c_str());
		
		for (int i = 1; i < _filename_list.size(); i++)
		{
			ReadShellComputeDisplacement(_filename_list[i]);
		}

		AggregateAllDisplacements();
	}



}


