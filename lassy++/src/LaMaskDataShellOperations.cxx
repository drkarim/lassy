#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaMaskDataShellOperations.h"
#include "../include/LaShell.h"

using namespace std;


LaMaskDataShellOperations::LaMaskDataShellOperations()
{
	_data_shell = new LaShell(); 
	_mask_shell = new LaShell(); 
	_output_la  = new LaShell();
	
	_which_operation = MEAN;
}

LaMaskDataShellOperations::~LaMaskDataShellOperations() {

	delete _data_shell; 
	delete _mask_shell; 
	delete _output_la;
}


void LaMaskDataShellOperations::SetInputData(LaShell* shell) {

	_data_shell = shell; 
}

void LaMaskDataShellOperations::SetInputData2(LaShell* shell) {

	_mask_shell = shell; 
}

double LaMaskDataShellOperations::GetOutputValue()
{
	return _scalar_aggregate;
}

LaShell* LaMaskDataShellOperations::GetOutput() {
	return _output_la;
}

void LaMaskDataShellOperations::SetOperationToMean() 
{
	//cout << "\n\ntPerforming Mean ...\n";
	_which_operation = MEAN; 
}

void LaMaskDataShellOperations::SetOperationToMedian() 
{
	//cout << "\n\ntPerforming Mean ...\n";
	_which_operation = MEDIAN; 
}

void LaMaskDataShellOperations::SetOperationToStdev() 
{
	//cout << "\n\ntPerforming Mean ...\n";
	_which_operation = STDEV; 
}



void LaMaskDataShellOperations::Update() {

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	
	
	vtkSmartPointer<vtkPolyData> Data_Poly = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> Mask_Poly = vtkSmartPointer<vtkPolyData>::New();
	

	vtkSmartPointer<vtkFloatArray> Scalars_Data = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkFloatArray> Scalars_Mask = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkIntArray> Output_Poly_Scalar = vtkSmartPointer<vtkIntArray>::New();

	_data_shell->GetMesh3D(Data_Poly);
	Scalars_Data = vtkFloatArray::SafeDownCast(Data_Poly->GetPointData()->GetScalars()); 

	_mask_shell->GetMesh3D(Mask_Poly);
	Scalars_Mask = vtkFloatArray::SafeDownCast(Mask_Poly->GetPointData()->GetScalars());
	
	
	double m[3];
	vector<double> data_in_mask; 

	// initialise output scalars
	for (int i=0;i<Data_Poly->GetNumberOfPoints();i++)
	{
		Output_Poly_Scalar->InsertNextTuple1(0);
	}

	vtkSmartPointer<vtkPointLocator> Target_Poly_PointLocator = vtkSmartPointer<vtkPointLocator>::New();
	Target_Poly_PointLocator->SetDataSet(Data_Poly);
	Target_Poly_PointLocator->AutomaticOn();
	Target_Poly_PointLocator->BuildLocator();

	// Nested for loop, for every point in Data poly, search whether it is masked by 
	// masked poly
	for (vtkIdType i = 0; i < Mask_Poly->GetNumberOfPoints(); ++i) {

		Mask_Poly->GetPoint(i, m);
		
		vtkIdType id_on_data_poly = Target_Poly_PointLocator->FindClosestPoint(m);
		
		if (id_on_data_poly > 0)
		{
			double data = Scalars_Data->GetTuple1(id_on_data_poly);
			data_in_mask.push_back(data);
			Output_Poly_Scalar->SetTuple1(id_on_data_poly, 1);
		}
		
	
		
	}	// end outer for loop 



	switch (_which_operation)
	{
		double mean, median, stdev; 

		case MEAN: 
			mean = MathBox::CalcMean(data_in_mask);
			_scalar_aggregate = mean;
			break;
		
		case MEDIAN: 
			median = MathBox::CalcMedian(data_in_mask);
			_scalar_aggregate = median;
			break;

		case STDEV: 
			mean = MathBox::CalcMean(data_in_mask);
			stdev = MathBox::CalcStd(data_in_mask, mean);
			_scalar_aggregate = stdev;
			break;
		

	}

	vtkSmartPointer<vtkPolyData> Output_Poly = vtkSmartPointer<vtkPolyData>::New();
	Output_Poly->DeepCopy(Data_Poly);

	Output_Poly->GetPointData()->SetScalars(Output_Poly_Scalar);

	_output_la->SetMesh3D(Output_Poly);
	

}