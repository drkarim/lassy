#define HAS_VTK 1

#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellDeformationWithField.h"

using namespace std;


LaShellDeformationWithField::LaShellDeformationWithField()
{
	_source_la = new LaShell();
	_output_la = new LaShell();
	_SourcePolyData = vtkSmartPointer<vtkPolyData>::New();
	
}

LaShellDeformationWithField::~LaShellDeformationWithField() {
	
}

LaShellDeformationWithField::SetInputData(LaShell* shell)
{
    _source_la = shell; 
    _source_la->GetMesh3D(_SourcePolyData);
}



LaShell* LaShellDeformationWithField::GetOutput() {
	return _output_la;
}



void LaShellDeformationWithField::Update() {

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	


}
