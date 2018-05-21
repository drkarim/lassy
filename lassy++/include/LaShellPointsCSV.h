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
#include <vtkVertexGlyphFilter.h>

#include <string>
#include <sstream>
#include <vector>

#include "CSVReader.h"
#include "LaShellAlgorithms.h"
#include "LaShell.h"

using namespace std;

class LaShellPointsCSV : public LaShellAlgorithms {

protected: 

	LaShell* _source_la;
    LaShell* _output_la;

    ifstream _csvfilestream;
    vtkSmartPointer<vtkPoints> _point_set;  // csv points - points listed in csv file
    
    bool _containers_set;
    int _copy_method;
    int _scaling_factor;
    int _neighbour_radius;
    double _insert_scalar_value;
    const char* _new_scalar_array_name;

    // containers 
    vector<vector<int> > _neighbour_point_set;  // points on shell neighbour to csv points
    vector<double> _scalars;                
    vector<int> _closest_point_ids;         // stores the  closest point's id to each xyz point listed on csv
    vector<int> _earmark_point;
    
    
public:
		
	void SetInputData(LaShell* shell);
	void ReadCSVFile(const char* input_fn);

    void SetCopyMethodToNeighbourCopy();
    void SetCopyMethodToPointCopy();
    void SetNeighbourRadius(int);
    void SetArrayName(const char* array_name);
    void SetInsertScalarValue(double val);
    void SetScalingFactor(int scale);

    void LocateNeighboursOfPoints();

    void InsertScalarData();

    void VisualisePoints();
	void Update();
	//LaShell* GetOutput();
    
    LaShell* GetOutput();

	LaShellPointsCSV();
	~LaShellPointsCSV();

private: 
	void LocatePoints();
    void InitNeighbourPointListingContainer();
	
	
}; 