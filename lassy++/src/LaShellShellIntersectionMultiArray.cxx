#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellShellIntersectionMultiArray.h"

using namespace std;


LaShellShellIntersectionMultiArray::LaShellShellIntersectionMultiArray()
{
	_which_direction = 1;
	_which_mapping = COPY_USING_NORMAL; 
	_output_la = new LaShell(); 
	
	
}

LaShellShellIntersectionMultiArray::~LaShellShellIntersectionMultiArray()
{
	delete _output_la; 
}

void LaShellShellIntersectionMultiArray::SetInputData(LaShell* shell)
{
	_source_la = shell; 
}

void LaShellShellIntersectionMultiArray::SetInputData2(LaShell* shell)
{
	_target_la = shell; 
}

LaShell* LaShellShellIntersectionMultiArray::GetOutput()
{
	return _output_la; 
}



void LaShellShellIntersectionMultiArray::SetDirectionToOppositeNormal()
{
	_which_direction = -1; 
}


void LaShellShellIntersectionMultiArray::SetCopyScalarsUsingNormal()
{
	_which_mapping = COPY_USING_NORMAL;
}

void LaShellShellIntersectionMultiArray::SetCopyScalarsUsingPointid()
{
	_which_mapping = COPY_USING_POINTID;
}


void LaShellShellIntersectionMultiArray::Update()
{
	double pN[3], pStart[3], pEnd[3], tolerance = .001, max_dist=1000;

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

	vtkSmartPointer<vtkPolyData> Source_Poly = vtkSmartPointer<vtkPolyData>::New(); 
	vtkSmartPointer<vtkPolyData> Target_Poly = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> Output_Poly = vtkSmartPointer<vtkPolyData>::New();


	_source_la->GetMesh3D(Source_Poly);
	_target_la->GetMesh3D(Target_Poly);

	vtkSmartPointer<vtkPointLocator> Source_Poly_PointLocator = vtkSmartPointer<vtkPointLocator>::New();
	Source_Poly_PointLocator->SetDataSet(Source_Poly);
	Source_Poly_PointLocator->AutomaticOn();
	Source_Poly_PointLocator->BuildLocator();
	

	Output_Poly->DeepCopy(Target_Poly);
	
    int numberOfPointArrays = Source_Poly->GetPointData()->GetNumberOfArrays();
    cout << "Total arrays to transfer from target 2 source = " << numberOfPointArrays << endl;
    
    vector<vtkSmartPointer<vtkFloatArray> > source_arrays;
    vector<vtkSmartPointer<vtkFloatArray> > source_arrays_in_target;
    vector<string> source_array_names; 

    // create copy of arrays from source
    for (int i=0;i<numberOfPointArrays;i++)
    {
        source_arrays.push_back(vtkSmartPointer<vtkFloatArray>::New());
        source_array_names.push_back(Source_Poly->GetPointData()->GetArray(i)->GetName());
    }
    cout << "\nfinished creating copy array structures ... " << endl;
    
    // create copy of arrays to copy from source to target 
    for (int i=0;i<numberOfPointArrays;i++)
    {
        source_arrays_in_target.push_back(vtkSmartPointer<vtkFloatArray>::New());        
    }
    cout << "\nfinished initialising copy arrays ... " << endl;
    
    // Empty array containers for source to target 
    for (int i=0;i<numberOfPointArrays;i++)
    {
        for (vtkIdType j = 0; j < Output_Poly->GetNumberOfPoints(); ++j) {
            source_arrays_in_target[i]->InsertNextValue(0);            // empty value, will be assigned later 
            
        }
        source_arrays_in_target[i]->SetName(source_array_names[i].c_str());
    }
    cout << "\nfinished setting empty copy arrays ... total points in target = " << Target_Poly->GetNumberOfPoints() << endl;
	

	for (vtkIdType i = 0; i < Output_Poly->GetNumberOfPoints(); ++i) {
		cout << i << ",";
		Target_Poly->GetPoint(i, pStart); 
		//Target_pNormals->GetTuple(i, pN);
        
		//LaShellShellIntersection::GetFiniteLine(pStart, pN, max_dist, _which_direction, pEnd);

		// find intersection with source 
        switch (_which_mapping) 
        {
            case COPY_USING_NORMAL:
            {
                vtkIdType iD = Source_Poly_PointLocator->FindClosestPoint(pStart);

                if (iD >= 0)		 // there are intersections
                {
                    
                    // copy every array value 
                    for (int k=0;k<numberOfPointArrays;k++)
                    {
                        float value_at_source = source_arrays[k]->GetTuple1(iD); 
                        source_arrays_in_target[k]->SetTuple1(i, value_at_source); 
                    }
                    
                }
                break;
            }
            case COPY_USING_POINTID: 
            {
                // copy every array value 
                for (int k=0;k<numberOfPointArrays;k++)
                {
                    float value_at_source = source_arrays[k]->GetTuple1(i); 
                    source_arrays_in_target[k]->SetTuple1(i, value_at_source); 
                }
                break; 
            }
        }
		
	
	}
     
    for (int k=0;k<numberOfPointArrays;k++)
    {
        Output_Poly->GetPointData()->AddArray(source_arrays_in_target[k]);
    }
    _output_la->SetMesh3D(Output_Poly);	
	
}



