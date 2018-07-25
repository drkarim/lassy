/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellPointsCSV reads a CSV file containing 3D points and scalar values
*	For each 3D point, it finds corresponding positionss on the shell and assigns scalar values to these vertices 
*   An example usage can be found udner applications/csvshell.cxx
*/
#pragma once

#define POINT_COPY 1 
#define NEIGHBOUR_COPY 2 


#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointLocator.h>


#include <string>
#include <sstream>
#include <vector>

#include "CSVReader.h"
#include "LaShellAlgorithms.h"
#include "LaShell.h"

using namespace std;

class LaShell2ShellPointsCSV : public LaShellAlgorithms {

protected: 

	LaShell* _source_la;
    LaShell* _target_la;
    LaShell* _source_in_target_la;

    ifstream _csvfilestream;
    vtkSmartPointer<vtkPoints> _point_set;  // csv points - points listed in csv file
    
    int _scaling_factor;
    
    // containers 
    vector<int> _closest_point_ids_in_source;         // stores the  closest point's id to each xyz point listed on csv
    vector<int> _closest_point_ids_in_target;
    char* _csv_filename;
    
public:
		
	void SetSourceData(LaShell* shell);
    void SetTargetData(LaShell* shell);
    void SetSourceInTargetData(LaShell* shell);
    void SetOutputFileName(char* fn);

	void ReadCSVFile(const char* input_fn);

    void LocateCSVPointsInSource();
    void LocateSourcePointsOnTarget();

    void Update();
	LaShell2ShellPointsCSV();
	~LaShell2ShellPointsCSV();

	
	
}; 