/*
*      Author:
*      Dr. Rashed Karim
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk
*      Copyright (c) 2017
*/

/*
*	The LaShellShellIntersection class computes the thickness between two shells indicated by source and target
*	For more information: https://www.vtk.org/doc/nightly/html/classvtkPolyData.html
*
*/
#pragma once
#define HAS_VTK 1
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataReader.h>
#include <vtkFileOutputWindow.h>
#include <vtkPolyDataWriter.h>
#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkModifiedBSPTree.h>
#include <string>
#include "LaShellAlgorithms.h"


using namespace std; 


class LaShellShellIntersection : public LaShellAlgorithms {
private:
	LaShell* _source_la; 
	LaShell* _target_la; 
	LaShell* _output_la;
	
	double _which_direction;
protected:
	
	static double GetEuclidean(double* p1, double* p2); 
	static void GetFiniteLine(double* start, double* direction, double max_distance, double which_direction, double* end);
	
	
public:
	
	LaShellShellIntersection();
	~LaShellShellIntersection();


	static LaShellShellIntersection *New();
	
	void SetInputData(LaShell* shell);
	void SetInputData2(LaShell* shell);
	
	void SetDirectionToOppositeNormal();
	

	void Update();

	LaShell* GetOutput();

	
}; 