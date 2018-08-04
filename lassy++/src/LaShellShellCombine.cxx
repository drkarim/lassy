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
    _write_to_field_data = false; 
    _output_scalar_name = "comb_scalar";
	
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

void LaShellShellCombine::SetWriteDataToField()
{
    _write_to_field_data= true; 
}

int LaShellShellCombine::FindScalarArray(LaShell* shell, const char* array_name)
{
    int location = -1;
    
    vtkIdType numberOfPointArrays;

    vtkSmartPointer<vtkPolyData> Source_Poly1 = vtkSmartPointer<vtkPolyData>::New();
    shell->GetMesh3D(Source_Poly1);

    if (!_write_to_field_data)
        numberOfPointArrays = Source_Poly1->GetPointData()->GetNumberOfArrays();
    else 
        numberOfPointArrays = Source_Poly1->GetFieldData()->GetNumberOfArrays();

	for(vtkIdType i = 0; i < numberOfPointArrays; i++)
    {
		//cout << "Array " << i << ", name = " << Source_Poly1->GetPointData()->GetArray(i)->GetName() << endl;
        if (!_write_to_field_data) {
            if (strcmp(Source_Poly1->GetPointData()->GetArray(i)->GetName(), array_name) == 0) {
                //cout << "Found array at location " << i << endl;
                location = i; 
            }
        }
        else {
             if (strcmp(Source_Poly1->GetFieldData()->GetArray(i)->GetName(), array_name) == 0) {
                //cout << "Found array at location " << i << endl;
                location = i; 
            }
        }
	}

    return location;

}

bool LaShellShellCombine::SetScalarArrayNames(const char* array_name1, const char* array_name2)
{
    int location1 = FindScalarArray(_source_la_1, array_name1); 
    int location2 = FindScalarArray(_source_la_2, array_name2); 

    if (location1 < 0 || location2 < 0)
    {
        if (location1 < 0)
            cerr << "The array with name: " << array_name1 << " does not exist" << endl;
        else 
            cerr << "The array with name: " << array_name2 << " does not exist" << endl;
        return false;
    }
    else { 
        _scalar_array_location_in_source1 = location1; 
        _scalar_array_location_in_source2 = location2; 
        cout << "Array with this name exists at location w/index " << location1 << ", and in second mesh at location w/index "<< location2 << endl;
        return true;
    }

    
}

void LaShellShellCombine::SetOutputScalarName(string array_name)
{
    _output_scalar_name = array_name;
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
	
    vtkSmartPointer<vtkDoubleArray> Scalars_Poly1 = vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> Scalars_Poly2 = vtkSmartPointer<vtkDoubleArray>::New();
	
    _source_la_1->GetMesh3D(Source_Poly1);
    _source_la_2->GetMesh3D(Source_Poly2);

    if (!_write_to_field_data) {
	    Scalars_Poly1 = vtkDoubleArray::SafeDownCast(Source_Poly1->GetPointData()->GetArray(_scalar_array_location_in_source1));
        Scalars_Poly2 = vtkDoubleArray::SafeDownCast(Source_Poly2->GetPointData()->GetArray(_scalar_array_location_in_source2));
    }
    else { 
        Scalars_Poly1 = vtkDoubleArray::SafeDownCast(Source_Poly1->GetFieldData()->GetArray(_scalar_array_location_in_source1));
        Scalars_Poly2 = vtkDoubleArray::SafeDownCast(Source_Poly2->GetFieldData()->GetArray(_scalar_array_location_in_source2));
    }

    vtkSmartPointer<vtkDoubleArray> Output_Poly_Scalar = vtkSmartPointer<vtkDoubleArray>::New();
	//Output_Poly_Scalar->SetNumberOfComponents(1);


    Output_Poly_Scalar->SetName(_output_scalar_name.c_str());
    cout << "New array name = " << _output_scalar_name << endl;
cout << "Array size = something else" << Scalars_Poly1->GetNumberOfComponents() << endl;
    for(vtkIdType i = 0; i < Scalars_Poly1->GetNumberOfTuples(); i++)
    {   
        float s1, s2;
        //cout << "s1=" << s1 << ", s2=" << s2 << endl;
        s1 = Scalars_Poly1->GetValue(i); 
        
        if ( i < Scalars_Poly2->GetNumberOfTuples()) {
            s2 = Scalars_Poly2->GetValue(i); 
        
            if ( s1 > 0 && s2 > 0)
            {
                if (_overlap_preference == WHEN_OVERLAP_USE_SOURCE1) {
                    Output_Poly_Scalar->InsertNextValue(s1); 
                }
                else if (_overlap_preference == WHEN_OVERLAP_USE_SOURCE2) {
                    Output_Poly_Scalar->InsertNextValue(s2); 
                }
            }
            else if (s1 > 0)
            {
                Output_Poly_Scalar->InsertNextValue(s1);
            }
            else if (s2 > 0)
            {
                Output_Poly_Scalar->InsertNextValue(s2);
            }
            else {
                Output_Poly_Scalar->InsertNextValue(0);
            }
        }
        
    }

    Output_Poly->DeepCopy(Source_Poly1);

    if (!_write_to_field_data) {
       // Output_Poly->GetPointData()->SetScalars(Output_Poly_Scalar);
        Output_Poly->GetPointData()->AddArray(Output_Poly_Scalar);
    }
    else {
        Output_Poly->GetFieldData()->AddArray(Output_Poly_Scalar);
    }

    _output_la->SetMesh3D(Output_Poly);
}