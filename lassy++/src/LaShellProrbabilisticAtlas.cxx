#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellProbabilisticAtlas.h"

using namespace std;


LaShellProbabilisticAtlas::LaShellProbabilisticAtlas()
{
	_source_la = new LaShell();
	_target_la = new LaShell();
	_output_la = new LaShell();
	_SourcePolyData = vtkSmartPointer<vtkPolyData>::New();
	_num_targets = ONE_TARGET;
	_num_targets_read = 0; 
	_aggregate_method = AGGREGATE_MEDIAN;
	_total_targets = 0;
}

LaShellProbabilisticAtlas::~LaShellProbabilisticAtlas() {
	
}

void LaShellProbabilisticAtlas::SetInputMultipleTargets(char* name_list)
{
	LaShellShellDisplacement::SetInputMultipleTargets(name_list);
}


void LaShellProbabilisticAtlas::SetInputData(LaShell* shell) {
	
	LaShellShellDisplacement::SetInputData(shell);
}

void LaShellProbabilisticAtlas::SetInputData2(LaShell* shell) {
	cout << "Fatal warning: Functionality not implemented yet!" << endl; 
}

void LaShellProbabilisticAtlas::SetAggregateMethodToMedian()
{
	_aggregate_method = AGGREGATE_MEDIAN; 
}

void LaShellProbabilisticAtlas::SetAggregateMethodToMean()
{
	_aggregate_method = AGGREGATE_MEAN;
}



LaShell* LaShellProbabilisticAtlas::GetOutput() {
	return _output_la;
}


double LaShellProbabilisticAtlas::GetEuclidean(double* p1, double* p2)
{
	LaShellShellDisplacement::GetEuclidean(p1, p2);
}

bool LaShellProbabilisticAtlas::ReadShellNameList(const char* fn)
{
	LaShellShellDisplacement::ReadShellNameList(fn);
}



void LaShellShellDisplacement::ReadShellComputeDisplacement(string poly_data_fn)
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

	_displacements.resize(_SourcePolyData->GetNumberOfPoints(), vector<double>(_total_targets));

	double source_vertex[3], target_vertex[3];
	
	for (vtkIdType i = 0; i < _SourcePolyData->GetNumberOfPoints(); ++i) {

		_SourcePolyData->GetPoint(i, source_vertex);

		vtkIdType id_on_target = Target_Poly_PointLocator->FindClosestPoint(source_vertex);

		TargetPolyData->GetPoint(id_on_target, target_vertex);
		double displacement = GetEuclidean(source_vertex, target_vertex);

		if (i < _displacements.size() && _num_targets_read < _displacements[i].size())
		{
			_displacements[i][_num_targets_read] = displacement; 
		}
		
	}

	_num_targets_read++; 

	cout << "completed!" << endl;
}


void LaShellShellDisplacement::AggregateAllDisplacements()
{
	
	vtkSmartPointer<vtkPolyData> shell_poly = vtkSmartPointer<vtkPolyData>::New();
	_source_la->GetMesh3D(shell_poly); 
	
	cout << "Aggregating displacements .. " << endl;

	cout << "Size of 2D container = " << _displacements.size(); 

	/*for (int i = 0; i < _displacements.size(); ++i) {
		cout << "\ni=" << i << ", size=" << _displacements[i].size() << endl; 
	}*/

	// prepare output 
	vtkSmartPointer<vtkPolyData> OutputPoly = vtkSmartPointer<vtkPolyData>::New();
	OutputPoly->DeepCopy(shell_poly);

	vtkSmartPointer<vtkFloatArray> output_scalars = vtkSmartPointer<vtkFloatArray>::New();
	output_scalars->SetNumberOfComponents(1);

	for (int i = 0; i < shell_poly->GetNumberOfPoints(); ++i) {

		double atlas_value = -1;
		if (_aggregate_method == AGGREGATE_MEDIAN)
		{
			// computing mean 
			double median = MathBox::CalcMedian(_displacements[i]);
			atlas_value = median;
		}
		else if (_aggregate_method == AGGREGATE_MEAN)
		{
			// compute median 
			double mean = MathBox::CalcMean(_displacements[i]);
			atlas_value = mean;
		}


		output_scalars->InsertNextTuple1(atlas_value);
	}

	OutputPoly->GetPointData()->SetScalars(output_scalars);

	_output_la->SetMesh3D(OutputPoly);
}



void LaShellShellDisplacement::Update() {

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


