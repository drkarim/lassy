/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaImageMarkPoinst reads a CSV file containing 3D points and plots them 
*   on the image 
*/
#pragma once

#define XY 1 
#define YZ 2 
#define XZ 3
#define XYZ 4 

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
#include "LaImageAlgorithms.h"
#include "LaImageFloat.h"
#include "itkNeighborhoodIterator.h"

using namespace std;


class LaImageHoleFilling : public LaImageAlgorithms {

protected: 

    int _iter; 
    const char* _output_fn;
    LaImageFloat* _input_img;
    int _min_neighbours;
    int _interpolate_direction;

public:
		
	void SetInputData(LaImageFloat* img);
    void SetNumberOfIterations(int iterations); 
    void SetOutputFileName(const char* output_fn);
    void SetMinNeighboursForMean(int size);
	void Update();
    void SetInterpolateDirection(int dir);
	//LaShell* GetOutput();

    
    

	LaImageHoleFilling();
	~LaImageHoleFilling();

	
	
}; 