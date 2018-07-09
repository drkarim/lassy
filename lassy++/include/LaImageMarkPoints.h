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
#include "LaImageAlgorithms.h"
#include "LaImage.h"

using namespace std;

class LaImageMarkPoints : public LaImageAlgorithms {

protected: 

	LaImage* _input_img;
     typedef itk::Image< unsigned short, 3 >  ImageType;

    ifstream _csvfilestream;
    vtkSmartPointer<vtkPoints> _point_set;  // csv points - points listed in csv file
    vtkSmartPointer<vtkPoints> _point_set_t;  // csv points - closest point set (also in csv file)
    double _scaling_factor;

    
    const char* _csv_filename;

    
public:
		
	void SetInputData(LaImage* img);
    void SetInputCSVFileName(const char* csv_fn);

    void SetScalingFactor(double scale);

    

	void Update();
	//LaShell* GetOutput();
    
    LaImage* GetOutput();

	LaImageMarkPoints();
	~LaImageMarkPoints();

private: 
	void MarkPoint(ImageType::IndexType pixelIndex, int markerpointsize);
    void ReadCSVFile();
	
	
}; 