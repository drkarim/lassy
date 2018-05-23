#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellShellCombine.h"

using namespace std;


LaShellShellCombine::LaShellShellCombine()
{
	_output_la = new LaShell();
	_overlap_preference = WHEN_OVERLAP_USE_SOURCE1;
    _scalar_array_location_in_source1 = -1;
    _scalar_array_location_in_source2 = -1;
	
}


LaShellShellCombine::~LaShellShellCombine() {
	delete _output_la; 
}


void LaShellShellCombine::SetOverlapToSource1() {
    _overlap_preference = WHEN_OVERLAP_USE_SOURCE1;
}

void LaShellShellCombine::SetOverlapToSource2() {
    _overlap_preference = WHEN_OVERLAP_USE_SOURCE2;
}


void LaShellShellCombine::SetInputData(LaShell* shell)
{
    _source_la_1 = shell; 
}

void LaShellShellCombine::SetInputData2(LaShell* shell)
{
    _source_la_2 = shell; 
}


int LaShellShellCombine::FindScalarArray(LaShell* shell, const char* array_name)
{
    int location = -1;
    _scalar_array_name = array_name;

    vtkSmartPointer<vtkPolyData> Source_Poly1 = vtkSmartPointer<vtkPolyData>::New();
    shell->GetMesh3D(Source_Poly1);

    vtkIdType numberOfPointArrays = Source_Poly1->GetPointData()->GetNumberOfArrays();

	for(vtkIdType i = 0; i < numberOfPointArrays; i++)
    {
		//cout << "Array " << i << ", name = " << Source_Poly1->GetPointData()->GetArray(i)->GetName() << endl;
		if (strcmp(Source_Poly1->GetPointData()->GetArray(i)->GetName(), array_name) == 0) {
			//cout << "Found array at location " << i << endl;
			location = i; 
		}
	}

    return location;

}

bool LaShellShellCombine::SetScalarArrayName(const char* array_name)
{
    _scalar_array_name = array_name;
    int location1 = FindScalarArray(_source_la_1, array_name); 
    int location2 = FindScalarArray(_source_la_2, array_name); 

    if (location1 < 0 || location2 < 0)
    {
        cerr << "The array with name: " << array_name << " does not exist" << endl;
        return false;
    }
    else { 
        _scalar_array_location_in_source1 = location1; 
        _scalar_array_location_in_source2 = location2; 
        cout << "Array with this name exists at location w/index " << location1 << ", and in second mesh at location w/index "<< location2 << endl;
        return true;
    }

    
}

LaShell* LaShellShellCombine::GetOutput() {
	return _output_la;
}



void LaShellShellCombine::Update() 
{

	// VTK error logging 
	vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow = vtkSmartPointer<vtkFileOutputWindow>::New();
	fileOutputWindow->SetFileName("vtkLog.txt");
	vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
	if (outputWindow)
	{
		outputWindow->SetInstance(fileOutputWindow);
	}

	vtkSmartPointer<vtkPolyData> Output_Poly = vtkSmartPointer<vtkPolyData>::New();
	
	vtkSmartPointer<vtkPolyData> Source_Poly1 = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> Source_Poly2 = vtkSmartPointer<vtkPolyData>::New();
	
    vtkSmartPointer<vtkFloatArray> Scalars_Poly1 = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkFloatArray> Scalars_Poly2 = vtkSmartPointer<vtkFloatArray>::New();
	
    _source_la_1->GetMesh3D(Source_Poly1);
    _source_la_2->GetMesh3D(Source_Poly2);

	Scalars_Poly1 = vtkFloatArray::SafeDownCast(Source_Poly1->GetPointData()->GetArray(_scalar_array_location_in_source1));
    Scalars_Poly2 = vtkFloatArray::SafeDownCast(Source_Poly2->GetPointData()->GetArray(_scalar_array_location_in_source2));

    vtkSmartPointer<vtkFloatArray> Output_Poly_Scalar = vtkSmartPointer<vtkFloatArray>::New();
	//Output_Poly_Scalar->SetNumberOfComponents(1);
    Output_Poly_Scalar->SetName(_scalar_array_name);

    for(vtkIdType i = 0; i < Source_Poly1->GetNumberOfPoints(); i++)
    {   
        float s1 = Scalars_Poly1->GetTuple1(i); 
        float s2 = Scalars_Poly2->GetTuple1(i); 
        
        if ( s1 > 0 && s2 > 0)
        {
            if (_overlap_preference == WHEN_OVERLAP_USE_SOURCE1)
                Output_Poly_Scalar->InsertNextTuple1(s1); 
            else if (_overlap_preference == WHEN_OVERLAP_USE_SOURCE2)
                Output_Poly_Scalar->InsertNextTuple1(s2); 
        }
        else if (s1 > 0)
        {
            Output_Poly_Scalar->InsertNextTuple1(s1);
        }
        else if (s2 > 0)
        {
            Output_Poly_Scalar->InsertNextTuple1(s2);
        }
        else {
            Output_Poly_Scalar->InsertNextTuple1(0);
        }
    }

    Output_Poly->DeepCopy(Source_Poly1);
    Output_Poly->GetPointData()->SetScalars(Output_Poly_Scalar);

    _output_la->SetMesh3D(Output_Poly);
}