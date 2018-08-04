#define HAS_VTK 1

/* The Circle class (All source codes in one file) (CircleAIO.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include "../include/LaShellShellIntersectionMultiArray.h"

using namespace std;


LaShellShellIntersectionMultiArray::LaShellShellIntersectionMultiArray()
{
	
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
    cout << "Source set "  << endl;
}

void LaShellShellIntersectionMultiArray::SetInputData2(LaShell* shell)
{
	_target_la = shell; 
    cout << "Target set "  << endl;
}

LaShell* LaShellShellIntersectionMultiArray::GetOutput()
{
	return _output_la; 
}



void LaShellShellIntersectionMultiArray::SetCopyScalarsUsingNormal()
{
	_which_mapping = COPY_USING_NORMAL;
}

void LaShellShellIntersectionMultiArray::SetCopyScalarsUsingPointid()
{
	_which_mapping = COPY_USING_POINTID;
    cout << "Warning: using pointid to copy, meshes must have equal vertices\n";
}


void LaShellShellIntersectionMultiArray::Update()
{
	double pStart[3];
	vtkSmartPointer<vtkPolyData> Source_Poly = vtkSmartPointer<vtkPolyData>::New(); 
	vtkSmartPointer<vtkPolyData> Target_Poly = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> Output_Poly = vtkSmartPointer<vtkPolyData>::New();

	_source_la->GetMesh3D(Source_Poly);
	_target_la->GetMesh3D(Target_Poly);

	Output_Poly->DeepCopy(Target_Poly);
	
    int numberOfPointArraysInSource = Source_Poly->GetPointData()->GetNumberOfArrays();
    cout << "Total arrays to transfer from target to source = " << numberOfPointArraysInSource << endl;
    
    vector< vector<double> > source_arrays;
    vector<vector<double> > source_arrays_in_target;
    vector<string> source_array_names; 


    // create copy of arrays from source
    for (int i=0;i<numberOfPointArraysInSource;i++)
    {
        string str = Source_Poly->GetPointData()->GetArray(i)->GetName();
        
        source_array_names.push_back(str);
        
        vector<double> v(Source_Poly->GetPointData()->GetArray(i)->GetNumberOfTuples(), 0); 
        source_arrays.push_back(v);
        
        
    }
    cout << "Array allocation completed .. ";


    for (int i=0;i<numberOfPointArraysInSource;i++)
    {
        vtkSmartPointer<vtkDoubleArray> Source_Scalars = vtkSmartPointer<vtkDoubleArray>::New();
	    Source_Scalars = vtkDoubleArray::SafeDownCast(Source_Poly->GetPointData()->GetArray(i));
    
        for (int k=0;k<Source_Scalars->GetNumberOfTuples();k++)
        {
            double value = Source_Scalars->GetValue(k);
            source_arrays[i][k]= value;
        }

    //    cout << "In array " << Source_Poly->GetPointData()->GetArray(i)->GetName() << " Tuples = " << Source_Scalars->GetNumberOfTuples() << endl;
    
    }

    
    // create copy of arrays to copy from source to target 
    
    for (int i=0;i<numberOfPointArraysInSource;i++)
    {
        vector<double>  v(Target_Poly->GetNumberOfPoints(), 0);
        source_arrays_in_target.push_back(v);        
    }
    cout << "\nfinished initialising copy arrays ... " << endl;
    
    
    vtkSmartPointer<vtkPointLocator> Source_Poly_PointLocator = vtkSmartPointer<vtkPointLocator>::New();
	Source_Poly_PointLocator->SetDataSet(Source_Poly);
	Source_Poly_PointLocator->AutomaticOn();
	Source_Poly_PointLocator->BuildLocator();


	for (vtkIdType i = 0; i < Target_Poly->GetNumberOfPoints(); i++) {
		
		Target_Poly->GetPoint(i, pStart); 
		//Target_pNormals->GetTuple(i, pN);
        
		//LaShellShellIntersection::GetFiniteLine(pStart, pN, max_dist, _which_direction, pEnd);

		// find intersection with source 
        if (_which_mapping == COPY_USING_NORMAL) 
        {
            
            vtkIdType iD = Source_Poly_PointLocator->FindClosestPoint(pStart);

            if (iD >= 0 && iD < Source_Poly->GetNumberOfPoints())		 // there are intersections
            {
                // copy every array value 
                for (int k=0;k<numberOfPointArraysInSource;k++)
                {
                    // troubling line
                    if (iD < source_arrays[k].size())
                    {
                        
                        double value_at_source = source_arrays[k][iD];
                        
                        source_arrays_in_target[k][i] = value_at_source;
                    }
                }
                
            }
                
        }
        else if (_which_mapping == COPY_USING_POINTID)
        {
            // copy every array value 
            for (int k=0;k<numberOfPointArraysInSource;k++)
            {
                if (i < source_arrays[k].size()) {
                    double value_at_source = source_arrays[k][i];

                    if (i < source_arrays_in_target[k].size())
                        source_arrays_in_target[k][i] = value_at_source;
                }
            }
                
        }
    }
		

	
     
    for (int k=0;k<numberOfPointArraysInSource;k++)
    {
        vtkSmartPointer<vtkDoubleArray> new_scalar = vtkSmartPointer<vtkDoubleArray>::New(); 
        
        for (int i=0;i<source_arrays_in_target[k].size();i++)
        {
            new_scalar->InsertNextValue(source_arrays_in_target[k][i]);
        }
        new_scalar->SetName(source_array_names[k].c_str());
        Output_Poly->GetPointData()->AddArray(new_scalar);
        
    }
    
    _output_la->SetMesh3D(Output_Poly);	
	
}



