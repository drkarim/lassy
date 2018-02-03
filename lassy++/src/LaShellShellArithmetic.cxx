#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellShellArithmetic.h"

using namespace std;


LaShellShellArithmetic::LaShellShellArithmetic()
{
	_output_la = new LaShell();
	_operation = ADD;
	_factor = 1.0; 
}

LaShellShellArithmetic::~LaShellShellArithmetic() {
	delete _output_la; 
}


void LaShellShellArithmetic::SetInputData(LaShell* shell) {
	_source_la_1 = shell; 
}

void LaShellShellArithmetic::SetInputData2(LaShell* shell) {
	_source_la_2 = shell; 
}


LaShell* LaShellShellArithmetic::GetOutput() {
	return _output_la;
}

void LaShellShellArithmetic::SetArithmetricOperationToAdd() {
	_operation = ADD; 
}

void LaShellShellArithmetic::SetArithmetricOperationToSubtract() {
	_operation = SUBTRACT;
}

void LaShellShellArithmetic::SetArithmetricOperationToMultiply(){
	_operation = MULTIPLY;
}

void LaShellShellArithmetic::SetArithmetricOperationToDivide(){
	_operation = DIVIDE; 
}

void LaShellShellArithmetic::SetFactor(double factor) {
	_factor = factor; 
}

void LaShellShellArithmetic::Update() {

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	vtkSmartPointer<vtkPolyData> Source_Poly1 = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> Source_Poly2 = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> Output_Poly = vtkSmartPointer<vtkPolyData>::New();

	vtkSmartPointer<vtkFloatArray> Scalars_Poly1 = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkFloatArray> Scalars_Poly2 = vtkSmartPointer<vtkFloatArray>::New();

	_source_la_1->GetMesh3D(Source_Poly1);
	_source_la_2->GetMesh3D(Source_Poly2);
	
	vtkSmartPointer<vtkFloatArray> Output_Poly_Scalar = vtkSmartPointer<vtkFloatArray>::New();
	Output_Poly_Scalar->SetNumberOfComponents(1);
	Output_Poly->DeepCopy(Source_Poly1);

	Scalars_Poly1 = vtkFloatArray::SafeDownCast(Source_Poly1->GetPointData()->GetScalars()); 
	Scalars_Poly2 = vtkFloatArray::SafeDownCast(Source_Poly2->GetPointData()->GetScalars());



	for (vtkIdType i = 0; i < Source_Poly1->GetNumberOfPoints(); ++i) {

		double scalar1 = Scalars_Poly1->GetTuple1(i);
		double scalar2 = Scalars_Poly2->GetTuple1(i);
		double result = 0.0; 

		switch (_operation)
		{
			case ADD: 
				result = scalar1 + scalar2; 
				break;
			case SUBTRACT:
				result = scalar1 - scalar2; 
				break;
			case MULTIPLY:
				result = scalar1 * scalar2; 
				break;
			case DIVIDE:
				if (scalar2 != 0)
					result = scalar1 / scalar2;
				else
					result = 0; 
				break;

		}

		result = result*_factor; 
		Output_Poly_Scalar->InsertNextTuple1(result);
	}

	Output_Poly->GetPointData()->SetScalars(Output_Poly_Scalar);

	_output_la->SetMesh3D(Output_Poly);
}


