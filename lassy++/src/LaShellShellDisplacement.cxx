#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellShellDisplacement.h"

using namespace std;


LaShellShellDisplacement::LaShellShellDisplacement()
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

LaShellShellDisplacement::~LaShellShellDisplacement() {
	delete _source_la; 
	delete _target_la; 
	delete _output_la;
}

void LaShellShellDisplacement::SetInputMultipleTargets(char* name_list)
{
	stringstream ss; 
	ss << name_list; 
	_multiple_target_fn = ss.str(); 
	_num_targets = MULTIPLE_TARGETS; 
	_num_targets_read = 0;
}


void LaShellShellDisplacement::SetInputData(LaShell* shell) {
	_source_la = shell;
	_source_la->GetMesh3D(_SourcePolyData);
	
}

void LaShellShellDisplacement::SetInputData2(LaShell* shell) {
	cout << "Fatal warning: Functionality not implemented yet!" << endl; 
}

void LaShellShellDisplacement::SetAggregateMethodToMedian()
{
	_aggregate_method = AGGREGATE_MEDIAN; 
}

void LaShellShellDisplacement::SetAggregateMethodToMean()
{
	_aggregate_method = AGGREGATE_MEAN;
}



LaShell* LaShellShellDisplacement::GetOutput() {
	return _output_la;
}


double LaShellShellDisplacement::GetEuclidean(double* p1, double* p2)
{
	double sum;
	sum = ((p1[0] - p2[0])*(p1[0] - p2[0])) + ((p1[1] - p2[1])*(p1[1] - p2[1])) + ((p1[2] - p2[2])*(p1[2] - p2[2]));
	return sqrt(sum);
}

bool LaShellShellDisplacement::ReadShellNameList(const char* fn)
{
	ifstream infile(fn);
	cout << "Reading mutliple target filename list .. \n";
	std::string line;

	while (getline(infile, line))
	{
		istringstream iss(line);
		string filename;
		if (!(iss >> filename)) {
			cout << "Error reading file containing filenames, check format" << endl;
			return false;
		} // error
		else {
			cout << "Found " << filename << endl;
			_filename_list.push_back(filename);
			_total_targets++;
		}
		
		// process pair (a,b)
	}
	return true;
}

// take a small step inside shell, compare distance to test_point to small step outside shell 
int LaShellShellDisplacement::IsPointOutsideOrInsideShell(vtkIdType shell_point, double* test_point)
{
	double normal_vec[3], shell_point_xyz[3]; 
	double small_step = 0.1; 

	_SourcePolyData->GetPoint(shell_point, shell_point_xyz);
	_SourcePolyNormals->GetTuple(shell_point, normal_vec);

	double small_step_inside_shell[3], small_step_outside_shell[3]; 

	small_step_outside_shell[0] = shell_point_xyz[0] + (small_step*normal_vec[0]);
	small_step_outside_shell[1] = shell_point_xyz[1] + (small_step*normal_vec[1]);
	small_step_outside_shell[2] = shell_point_xyz[2] + (small_step*normal_vec[2]);

	small_step_inside_shell[0] = shell_point_xyz[0] + (-1*small_step*normal_vec[0]);
	small_step_inside_shell[1] = shell_point_xyz[1] + (-1*small_step*normal_vec[1]);
	small_step_inside_shell[2] = shell_point_xyz[2] + (-1*small_step*normal_vec[2]);

	double test_to_inside_point = GetEuclidean(test_point, small_step_inside_shell); 
	double test_to_outside_point = GetEuclidean(test_point, small_step_outside_shell);

	if (test_to_inside_point < test_to_outside_point)
	{
		// inside point is closer to test point 
		return -1; 
	}
	else if (test_to_inside_point >= test_to_outside_point)
	{
		return 1; 
	}
	
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

	// compute normals for finding direction of displacement 
	vtkSmartPointer<vtkPolyDataNormals> Source_Poly_Normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	Source_Poly_Normals->ComputeCellNormalsOn();
	Source_Poly_Normals->SetInputData(_SourcePolyData);
	Source_Poly_Normals->Update();

	_SourcePolyNormals = vtkFloatArray::SafeDownCast(_SourcePolyData->GetPointData()->GetNormals());

	_displacements.resize(_SourcePolyData->GetNumberOfPoints(), vector<double>(_total_targets));

	//_displacement_direction.resize(_SourcePolyData->GetNumberOfPoints(), vector<double>(_total_targets));

	double source_vertex[3], target_vertex[3];
	
	for (vtkIdType i = 0; i < _SourcePolyData->GetNumberOfPoints(); ++i) {

		_SourcePolyData->GetPoint(i, source_vertex);

		vtkIdType id_on_target = Target_Poly_PointLocator->FindClosestPoint(source_vertex);

		TargetPolyData->GetPoint(id_on_target, target_vertex);
		double displacement = GetEuclidean(source_vertex, target_vertex);

		if (i < _displacements.size() && _num_targets_read < _displacements[i].size())
		{
			
			// check direction of displacement 
			int displacement_direction = IsPointOutsideOrInsideShell(i, target_vertex);
			//int displacement_direction = 1;
			_displacements[i][_num_targets_read] = displacement_direction*displacement;
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
		
		for (int i = 0; i < _filename_list.size(); i++)
		{
			ReadShellComputeDisplacement(_filename_list[i]);
		}

		AggregateAllDisplacements();
	}



}


