#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellStatistics.h"

using namespace std;


LaShellStatistics::LaShellStatistics()
{
	_operation = DO_STANDARD_STATS;
	_source_la = new LaShell();
}

LaShellStatistics::~LaShellStatistics() {
	
}


void LaShellStatistics::SetInputData(LaShell* shell) {
	_source_la = shell; 
}

void LaShellStatistics::SetOperationToStandardStats()
{
    _operation = DO_STANDARD_STATS;
}

double LaShellStatistics::GetOutputValue(const char* label)
{
    stringstream ss; 
    ss << label; 
    auto search = _stats.find(ss.str()); 
    if (search != _stats.end())
    {
        return search->second;      // second of the pair inserted is the value
    }
    else 
    {
        cout << "Wrong stats label used, see documentation!" << endl; 
        return -1e10;
    }
}

void LaShellStatistics::Update() {

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("LaShellStatistics_vtkError_Logging.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}
    vector<double> value_container; 

	vtkSmartPointer<vtkPolyData> Source_Poly1 = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkFloatArray> Scalars_Poly1 = vtkSmartPointer<vtkFloatArray>::New();
	
	_source_la->GetMesh3D(Source_Poly1);
	
	Scalars_Poly1 = vtkFloatArray::SafeDownCast(Source_Poly1->GetPointData()->GetScalars()); 

	for (vtkIdType i = 0; i < Source_Poly1->GetNumberOfPoints(); ++i) {

		double scalar1 = Scalars_Poly1->GetTuple1(i);
        value_container.push_back(scalar1); 
	}


    switch (_operation)
    {
        case DO_STANDARD_STATS: 
            double mean = MathBox::CalcMean(value_container);
            double median = MathBox::CalcMedian(value_container);
            double std = MathBox::CalcStd(value_container, mean);
            string mean_label = "mean";
            string median_label = "median";
            string std_label = "std";
            _stats.insert(pair<string, double>(mean_label, mean));
            _stats.insert(pair<string, double>(median_label, median));
            _stats.insert(pair<string, double>(std_label, std));
        break; 
    }
    
}


