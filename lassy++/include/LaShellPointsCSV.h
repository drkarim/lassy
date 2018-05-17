/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellPointsCSV reads a CSV file containing 3D points 
*	that correspond to a shell's vertices and assign scalars to these vertices 
*   Note that the points are not required to correspond exactly to the shell vertices
*   The code supports multi-array scalars for meshes 
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
#include <vtkVertexGlyphFilter.h>

#include <string>
#include <sstream>

#include "CSVReader.h"
#include "LaShellAlgorithms.h"
#include "LaShell.h"


class LaShellPointsCSV : public LaShellAlgorithms {

protected: 

	LaShell* _source_la;
	LaShell* _target_la;
    LaShell* _output_la;
    ifstream _csvfilestream;
    vtkSmartPointer<vtkPoints> _point_set; 
    vector<double> _scalars;
    vector<int> _closest_point_ids; 
    int _copy_method;
 
public:
		
	void SetInputData(LaShell* shell);
	void ReadCSVFile(const char* input_fn);

    void SetCopyMethodToNeighbourCopy();
    void SetCopyMethodToPointCopy();

    void VisualisePoints();
	void Update();
	//LaShell* GetOutput();
    
    

	LaShellPointsCSV();
	~LaShellPointsCSV();

private: 
	void LocatePoints();
	
	
}; 